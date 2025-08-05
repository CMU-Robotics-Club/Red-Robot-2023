import csv
from dataclasses import dataclass
from typing import List

@dataclass
class BomRow:
    reference: List[str]
    value: str
    footprint: str
    description: str
    pn: str

    @classmethod
    def from_kicad_row(cls, row):
        reference, value, footprint, description, pn = row
        reference = reference.split(',')
        return cls(reference, value, footprint, description, pn)
    
    def jlc_row(self):
        comment = f'{self.value}: {self.description}'
        return [comment, ','.join(self.reference), self.footprint, self.pn]


def read_kicad_bom(path) -> List[BomRow]:
    result = []

    with open(path, 'r', newline='') as f:
        csvreader = csv.reader(f)
        header = next(csvreader)
        print(header)
        assert(header == ['Reference', 'Value', 'Footprint', 'Description', 'JLC P/N'])

        for row in csvreader:
            bom_row = BomRow.from_kicad_row(row)
            if bom_row.pn == '':
                print(f'SKIPPING {bom_row}')
                continue
            result.append(BomRow.from_kicad_row(row))
    
    return result

def convert_placement(inpath, outpath, refs):
    with open(inpath, 'r', newline='') as inf:
        csvreader = csv.reader(inf)
        assert(next(csvreader) == ['Ref', 'Val', 'Package', 'PosX', 'PosY', 'Rot', 'Side'])
        with open(outpath, 'w', newline='') as outf:
            csvwriter = csv.writer(outf)
            csvwriter.writerow(['Designator', 'Mid X', 'Mid Y', 'Layer', 'Rotation'])

            for row in csvreader:
                if row[0] in refs:
                    csvwriter.writerow([row[0], row[3], row[4], row[6], row[5]])

def main():
    bom = read_kicad_bom('fab/bom.csv')
    
    refs = set()
    for row in bom:
        for ref in row.reference:
            refs.add(ref)
    
    print('REFS:')
    print(refs)

    with open('fab/bom-jlc.csv', 'w', newline='') as of:
        csvwriter = csv.writer(of)
        csvwriter.writerow(['Comment', 'Designator', 'Footprint', 'LCSC Part Number'])
        for row in bom:
            csvwriter.writerow(row.jlc_row())

    print('Converted fab/bom.csv to fab/bom-jlc.csv')

    convert_placement('fab/bottom-pos.csv', 'fab/bottom-pos-jlc.csv', refs)
    convert_placement('fab/top-pos.csv', 'fab/top-pos-jlc.csv', refs)


if __name__ == '__main__':
    main()

