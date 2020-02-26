import os
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

CHORD_LOGS_PATH = '../chord/logs/'
PASTRY_LOGS_PATH = '../pastry/logs/'


def read(path):
    lis = []
    with open(path, 'r') as file:
        lines = file.readlines()
        lis = [int(i.strip()) for i in lines]
    return lis


def draw_all_graphs(initial_path, naming):
    num_keys_100 = read(initial_path + 'sim1_keys_before.txt')
    num_keys_500 = read(initial_path + 'sim2_keys_before.txt')
    num_keys_1000 = read(initial_path + 'sim3_keys_before.txt')
    path_lengths_100 = read(initial_path + 'sim1_before.txt')
    path_lengths_500 = read(initial_path + 'sim2_before.txt')
    path_lengths_1000 = read(initial_path + 'sim3_before.txt')

    num_keys_100_af = read(initial_path + 'sim1_keys_after.txt')
    num_keys_500_af = read(initial_path + 'sim2_keys_after.txt')
    num_keys_1000_af = read(initial_path + 'sim3_keys_after.txt')
    path_lengths_100_af = read(initial_path + 'sim1_after.txt')
    path_lengths_500_af = read(initial_path + 'sim2_after.txt')
    path_lengths_1000_af = read(initial_path + 'sim3_after.txt')

    # * Chord : Number of nodes vs. Number of keys per node
    data = [[] for i in range(10)]
    labels = ['' for i in range(10)]
    data[0], labels[0] = num_keys_100, 100
    data[4], labels[4] = num_keys_500, 500
    data[9], labels[9] = num_keys_1000, 1000

    plt.boxplot(data, labels=labels, showfliers=False,
                showmeans=True, meanline=True)
    plt.title('Num keys per node vs. num nodes')
    plt.xlabel('Number of Nodes')
    plt.ylabel('Num of keys per node')
    plt.savefig(
        'graphs/'+naming+'/num_keys_per_node_vs_num_nodes_before.svg', format='svg')
    plt.clf()

    data = [[] for i in range(10)]
    labels = ['' for i in range(10)]
    data[0], labels[0] = num_keys_100_af, 100
    data[4], labels[4] = num_keys_500_af, 500
    data[9], labels[9] = num_keys_1000_af, 1000

    plt.boxplot(data, labels=labels, showfliers=False,
                showmeans=True, meanline=True)
    plt.title('Num keys per node vs. num nodes')
    plt.xlabel('Number of Nodes')
    plt.ylabel('Num of keys per node')
    plt.savefig(
        'graphs/'+naming+'/num_keys_per_node_vs_num_nodes_after.svg', format='svg')
    plt.clf()

    # * Chord: PDF for num keys per node

    plt.title('PDF of num keys per node')
    plt.xlabel('Number of keys per node')
    plt.ylabel('Probability')

    ax = sns.distplot(num_keys_100, hist=False,
                      kde_kws={'bw': 1}, label='100 Nodes').set(xlim=(0))
    ax = sns.distplot(num_keys_500, hist=False,
                      kde_kws={'bw': 1}, label='500 Nodes').set(xlim=(0))
    ax = sns.distplot(num_keys_1000, hist=False,
                      kde_kws={'bw': 1}, label='1000 Nodes').set(xlim=(0))

    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming +
                '/pdf_num_keys_per_node_before.svg', format='svg')
    plt.clf()

    plt.title('PDF of num keys per node')
    plt.xlabel('Number of keys per node')
    plt.ylabel('Probability')

    ax = sns.distplot(num_keys_100_af, hist=False,
                      kde_kws={'bw': 1}, label='100 Nodes').set(xlim=(0))
    ax = sns.distplot(num_keys_500_af, hist=False,
                      kde_kws={'bw': 1}, label='500 Nodes').set(xlim=(0))
    ax = sns.distplot(num_keys_1000_af, hist=False,
                      kde_kws={'bw': 1}, label='1000 Nodes').set(xlim=(0))

    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming +
                '/pdf_num_keys_per_node_after.svg', format='svg')
    plt.clf()

    # * num keys per node vs. num keys
    data = []
    labels = []
    for i in range(1, 11):
        labels.append(i)
        data.append(read(initial_path + 'keys_var'+str(i)+"_keys_before.txt"))

    plt.boxplot(data, labels=labels, showfliers=False,
                showmeans=True, meanline=True)
    plt.title('Variation of number of keys per node')
    plt.xlabel('Number of Keys(x10000)')
    plt.ylabel('Number of keys per node')
    plt.savefig('graphs/'+naming+'/num_keys_per_node_dist.svg', format='svg')
    plt.clf()

    # * Path length vs. number of nodes

    data = [[] for i in range(10)]
    labels = ['' for i in range(10)]
    data[0], labels[0] = path_lengths_100, 100
    data[4], labels[4] = path_lengths_500, 500
    data[9], labels[9] = path_lengths_1000, 1000

    plt.boxplot(data, labels=labels, showfliers=False,
                showmeans=True, meanline=True)
    plt.title('Path length vs. num nodes')
    plt.xlabel('Number of Nodes')
    plt.ylabel('Path length')
    plt.savefig('graphs/'+naming +
                '/path_length_vs_num_nodes_before.svg', format='svg')
    plt.clf()

    data = [[] for i in range(10)]
    labels = ['' for i in range(10)]
    data[0], labels[0] = path_lengths_100_af, 100
    data[4], labels[4] = path_lengths_500_af, 500
    data[9], labels[9] = path_lengths_1000_af, 1000

    plt.boxplot(data, labels=labels, showfliers=False,
                showmeans=True, meanline=True)
    plt.title('Path length vs. num nodes')
    plt.xlabel('Number of Nodes')
    plt.ylabel('Path length')
    plt.savefig('graphs/'+naming +
                '/path_length_vs_num_nodes_after.svg', format='svg')
    plt.clf()

    # * PDF for path length

    plt.title('PDF of path length')
    plt.xlabel('Path length')
    plt.ylabel('Probability')
    ax = sns.distplot(path_lengths_100, hist=False, kde=True,
                      kde_kws={'bw': 1}, label='100 Nodes').set(xlim=(0))
    ax = sns.distplot(path_lengths_500, hist=False,
                      kde_kws={'bw': 1}, label='500 Nodes').set(xlim=(0))
    ax = sns.distplot(path_lengths_1000, hist=False,
                      kde_kws={'bw': 1}, label='1000 Nodes').set(xlim=(0))
    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming+'/pdf_path_len_before.svg', format='svg')
    plt.clf()

    plt.title('PDF of path length')
    plt.xlabel('Path length')
    plt.ylabel('Probability')
    ax = sns.distplot(path_lengths_100_af, hist=False, kde=True,
                      kde_kws={'bw': 1}, label='100 Nodes').set(xlim=(0))
    ax = sns.distplot(path_lengths_500_af, hist=False,
                      kde_kws={'bw': 1}, label='500 Nodes').set(xlim=(0))
    ax = sns.distplot(path_lengths_1000_af, hist=False,
                      kde_kws={'bw': 1}, label='1000 Nodes').set(xlim=(0))
    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming+'/pdf_path_len_after.svg', format='svg')
    plt.clf()

    # * Avg number of hops vs. nodes
    x,y = [100,500,1000,2000,5000,10000],[]
    for i in range(1, 7):
        lst = read(initial_path + 'nodes_var'+str(i)+"_before.txt")
        y.append(sum(lst) / len(lst))

    plt.plot(x,y,marker='o',label=naming)
    plt.plot(x,np.log(x),label='log(n)')

    plt.title('Avg. number of hops vs num nodes')
    plt.xlabel('Number of nodes')
    plt.xscale('log')
    plt.ylabel('Avg. num of hops')
    plt.legend()
    plt.savefig('graphs/'+naming+'/avg_hops_vs_node.svg', format='svg')
    plt.clf()


if __name__ == "__main__":

    draw_all_graphs(CHORD_LOGS_PATH, 'chord')
    draw_all_graphs(PASTRY_LOGS_PATH, 'pastry')
