import os
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

CHORD_LOGS_PATH = '../chord/logs/'
PASTRY_LOGS_PATH = '../pastry/logs/'


def read(path):
    lis = []
    with open(path, 'r') as file:
        lines = file.readlines()
        lis = [int(i.strip()) for i in lines]
    return lis


def mean(lst):
    return sum(lst)/len(lst)


def draw_all_graphs(initial_path, naming):
    num_keys_100 = read(initial_path + 'sim1_keys_before.txt')
    num_keys_500 = read(initial_path + 'sim2_keys_before.txt')
    num_keys_1000 = read(initial_path + 'sim3_keys_before.txt')
    path_lengths_100 = read(initial_path + 'sim1_before.txt')
    path_lengths_500 = read(initial_path + 'sim2_before.txt')
    path_lengths_1000 = read(initial_path + 'sim3_before.txt')
    avg_len_100 = mean(path_lengths_100)
    avg_len_500 = mean(path_lengths_500)
    avg_len_1000 = mean(path_lengths_1000)

    num_keys_100_af = read(initial_path + 'sim1_keys_after.txt')
    num_keys_500_af = read(initial_path + 'sim2_keys_after.txt')
    num_keys_1000_af = read(initial_path + 'sim3_keys_after.txt')
    path_lengths_100_af = read(initial_path + 'sim1_after.txt')
    path_lengths_500_af = read(initial_path + 'sim2_after.txt')
    path_lengths_1000_af = read(initial_path + 'sim3_after.txt')
    avg_len_100_af = mean(path_lengths_100_af)
    avg_len_500_af = mean(path_lengths_500_af)
    avg_len_1000_af = mean(path_lengths_1000_af)

    # * Number of nodes vs. Number of keys per node
    print(naming, 'Number of nodes vs. Number of keys per node')

    data = [[] for i in range(10)]
    labels = ['' for i in range(10)]
    data[0], labels[0] = num_keys_100, 100
    data[4], labels[4] = num_keys_500, 500
    data[9], labels[9] = num_keys_1000, 1000

    plt.boxplot(data, labels=labels, showfliers=False,
                showmeans=True, meanline=True)
    plt.title('Number of keys per node vs. number of nodes (Before deletion of half nodes)')
    plt.xlabel('Number of nodes')
    plt.ylabel('Num of keys per node')
    plt.savefig(
        'graphs/'+naming+'_num_keys_per_node_vs_num_nodes_before.svg', format='svg')
    plt.clf()

    data = [[] for i in range(10)]
    labels = ['' for i in range(10)]
    data[0], labels[0] = num_keys_100_af, 100
    data[4], labels[4] = num_keys_500_af, 500
    data[9], labels[9] = num_keys_1000_af, 1000

    plt.boxplot(data, labels=labels, showfliers=False,
                showmeans=True, meanline=True)
    plt.title('Number of keys per node vs. number of nodes (After deletion of half nodes)')
    plt.xlabel('Number of nodes')
    plt.ylabel('Num of keys per node')
    plt.savefig(
        'graphs/'+naming+'_num_keys_per_node_vs_num_nodes_after.svg', format='svg')
    plt.clf()

    # * PDF for num keys per node
    print(naming, 'PDF for num keys per node')

    plt.title('PDF of number of keys per node (Before deletion)')
    plt.xlabel('Number of keys per node')
    plt.ylabel('Probability')

    bins = np.arange(0, max(max(num_keys_100),
                            max(num_keys_500), max(num_keys_1000)) + 1.5) - 0.5
    sns.distplot(num_keys_100, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='100 Nodes').set(xlim=(0))
    sns.distplot(num_keys_500, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='500 Nodes').set(xlim=(0))
    sns.distplot(num_keys_1000, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='1000 Nodes').set(xlim=(0))

    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming +
                '_pdf_num_keys_per_node_before.svg', format='svg')
    plt.clf()

    plt.title('PDF of number of keys per node (After deletion)')
    plt.xlabel('Number of keys per node')
    plt.ylabel('Probability')

    bins = np.arange(0, max(max(num_keys_100_af),
                            max(num_keys_500_af), max(num_keys_1000_af)) + 1.5) - 0.5
    sns.distplot(num_keys_100_af, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='100 Nodes').set(xlim=(0))
    sns.distplot(num_keys_500_af, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='500 Nodes').set(xlim=(0))
    sns.distplot(num_keys_1000_af, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='1000 Nodes').set(xlim=(0))

    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming +
                '_pdf_num_keys_per_node_after.svg', format='svg')
    plt.clf()

    # * num keys per node vs. num keys
    print(naming, 'num keys per node vs. num keys')

    data = []
    labels = []
    for i in range(1, 11):
        labels.append(i)
        data.append(read(initial_path + 'keys_var'+str(i)+"_keys_before.txt"))

    plt.boxplot(data, labels=labels, showfliers=False,
                showmeans=True, meanline=True)
    plt.title('Variation of number of keys per node with increasing keys (1000 Nodes)')
    plt.xlabel('Number of keys (x10000)')
    plt.ylabel('Number of keys per node')
    plt.savefig('graphs/'+naming+'_num_keys_per_node_dist.svg', format='svg')
    plt.clf()

    # * Path length vs. number of nodes
    print(naming, 'Path length vs. number of nodes')

    data = [[] for i in range(10)]
    labels = ['' for i in range(10)]
    data[0], labels[0] = path_lengths_100, 100
    data[4], labels[4] = path_lengths_500, 500
    data[9], labels[9] = path_lengths_1000, 1000

    plt.boxplot(data, labels=labels, showfliers=False,
                showmeans=True, meanline=True)
    plt.title('Path length vs. num nodes (Before deletion)')
    plt.xlabel('Number of Nodes')
    plt.ylabel('Path length')
    plt.savefig('graphs/'+naming +
                '_path_length_vs_num_nodes_before.svg', format='svg')
    plt.clf()

    data = [[] for i in range(10)]
    labels = ['' for i in range(10)]
    data[0], labels[0] = path_lengths_100_af, 100
    data[4], labels[4] = path_lengths_500_af, 500
    data[9], labels[9] = path_lengths_1000_af, 1000

    plt.boxplot(data, labels=labels, showfliers=False,
                showmeans=True, meanline=True)
    plt.title('Path length vs. number of nodes (After deletion)')
    plt.xlabel('Number of nodes')
    plt.ylabel('Path length')
    plt.savefig('graphs/'+naming +
                '_path_length_vs_num_nodes_after.svg', format='svg')
    plt.clf()

    # * PDF for path length
    print(naming, 'PDF for path length')

    plt.title('PDF of path length (Before deletion)')
    plt.xlabel('Path length')
    plt.ylabel('Probability')
    bins = np.arange(0, max(max(path_lengths_100),
                            max(path_lengths_500), max(path_lengths_1000)) + 1.5) - 0.5
    sns.distplot(path_lengths_100, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='100 Nodes').set(xlim=(0))
    sns.distplot(path_lengths_500, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='500 Nodes').set(xlim=(0))
    sns.distplot(path_lengths_1000, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='1000 Nodes').set(xlim=(0))
    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming+'_pdf_path_len_before.svg', format='svg')
    plt.clf()

    plt.title('PDF of path length (After deletion)')
    plt.xlabel('Path length')
    plt.ylabel('Probability')
    bins = np.arange(0, max(max(path_lengths_100_af),
                            max(path_lengths_500_af), max(path_lengths_1000_af)) + 1.5) - 0.5
    sns.distplot(path_lengths_100_af, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='100 Nodes').set(xlim=(0))
    sns.distplot(path_lengths_500_af, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='500 Nodes').set(xlim=(0))
    sns.distplot(path_lengths_1000_af, hist=False, bins=bins,
                 kde_kws={'bw': 1}, label='1000 Nodes').set(xlim=(0))
    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming+'_pdf_path_len_after.svg', format='svg')
    plt.clf()

    # * Avg number of hops vs. nodes
    print(naming, 'Avg number of hops vs. nodes')

    x, y = [100, 500, 1000, 2000, 5000, 10000], []
    # logs = np.log(x)

    for i in range(1, 7):
        lst = read(initial_path + 'nodes_var'+str(i)+"_before.txt")
        y.append(sum(lst) / len(lst))

    popt, _ = curve_fit(func, x, y)

    # base = np.log(100)/y[0]
    # const = y[1]*base/np.log(500)
    plt.plot(x, y, marker='o', label=naming.title())
    plt.plot(x, popt[0]*np.log(x), marker='o', label='O(log(N))')

    plt.title('Avg. number of hops vs number of nodes')
    plt.xlabel('Number of nodes')
    plt.xscale('log')
    plt.ylabel('Avg. number of hops')
    plt.legend()
    plt.savefig('graphs/'+naming+'_avg_hops_vs_node.svg', format='svg')
    plt.clf()

    # * Avg. number of hops comparison with deletion
    print(naming, 'Avg. number of hops comparison with deletion')

    x, y = [100, 500, 1000], [avg_len_100, avg_len_500, avg_len_1000]
    y2 = [avg_len_100_af, avg_len_500_af, avg_len_1000_af]
    plt.plot(x, y, 'bo-', label='Before deletion')
    plt.plot(x, y2, 'ro-', label='After deletion')

    plt.title('Avg. number of hops vs number of nodes')
    plt.xlabel('Number of nodes')
    plt.xscale('log')
    plt.ylabel('Avg. number of hops')
    plt.legend()
    plt.savefig('graphs/'+naming +
                '_avg_hops_vs_node_deletions.svg', format='svg')
    plt.clf()

    # * Path length histogram
    print(naming, 'Path length histograms')

    plt.title('Path length Distribution Histogram (100 Nodes)')
    plt.xlabel('Path length')
    plt.ylabel('Number of searches')
    bins = np.arange(0, max(max(path_lengths_100),
                            max(path_lengths_100_af)) + 1.5) - 0.5

    plt.hist([path_lengths_100, path_lengths_100_af], bins,
             label=['Before Deletion', 'After Deletion'])
    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming+'_histogram_path_len_100.svg', format='svg')
    plt.clf()

    bins = np.arange(0, max(max(path_lengths_500),
                            max(path_lengths_500_af)) + 1.5) - 0.5
    plt.title('Path length Distribution Histogram (500 Nodes)')
    plt.xlabel('Path length')
    plt.ylabel('Number of searches')
    plt.hist([path_lengths_500, path_lengths_500_af], bins,
             label=['Before Deletion', 'After Deletion'])
    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming+'_histogram_path_len_500.svg', format='svg')
    plt.clf()

    bins = np.arange(0, max(max(path_lengths_1000),
                            max(path_lengths_1000_af)) + 1.5) - 0.5
    plt.title('Path length Distribution Histogram (1000 Nodes)')
    plt.xlabel('Path length')
    plt.ylabel('Number of searches')
    plt.hist([path_lengths_1000, path_lengths_1000_af], bins,
             label=['Before Deletion', 'After Deletion'])
    plt.legend(title='Legend')
    plt.savefig('graphs/'+naming+'_histogram_path_len_1000.svg', format='svg')
    plt.clf()


def func(x, a):
    return a*np.log(x)


if __name__ == "__main__":

    draw_all_graphs(CHORD_LOGS_PATH, 'chord')
    draw_all_graphs(PASTRY_LOGS_PATH, 'pastry')
