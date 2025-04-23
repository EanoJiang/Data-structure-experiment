import networkx as nx
import matplotlib.pyplot as plt

# 创建一个有向图
G = nx.DiGraph()

# 从文件中读取边的信息
edges = []
with open('data.txt', 'r') as file:
    for line in file:
        u, v, weight = map(int, line.strip().split(','))
        edges.append((u, v, {'weight': weight}))

# 添加边到图中
G.add_edges_from(edges)

# 绘制图
plt.figure(figsize=(10, 8))
pos = nx.spring_layout(G)
nx.draw(G, pos, with_labels=True, node_size=2000, node_color='lightblue', font_size=12, font_weight='bold', arrowsize=20)

# 显示边的权重
edge_labels = nx.get_edge_attributes(G, 'weight')
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)

# 设置标题并显示图形
plt.title("带权重的有向图")
plt.show()
