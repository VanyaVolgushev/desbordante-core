import desbordante
import pandas

TABLE = 'examples/datasets/players.csv'

def print_nars(nars):
    print('Total count of NARs:', len(nars))
    print('The first 10 NARs:')
    for nar in nars[:10]:
        print(nar, end=' ')
        print('confidence: ' + str(nar.get_qualities.confidence), end=' ')
        print('support: ' + str(nar.get_qualities.support))
        print('fitness: ' + str(nar.get_qualities.fitness), end='\n')

if __name__ == '__main__':
    algo = desbordante.nar.algorithms.Default()
    algo.load_data(table=(TABLE, ',', True))
    algo.execute()
    print_nars(algo.get_nars())
