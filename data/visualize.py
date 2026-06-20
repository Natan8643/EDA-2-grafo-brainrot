import json
import os
import webbrowser

def main():
    json_path = 'data/graph.json'
    html_path = 'data/graph.html'

    if not os.path.exists(json_path):
        print(f"Erro: O arquivo '{json_path}' não existe. Rode o programa C++ primeiro para gerá-lo.")
        return

    # Lê os dados gerados pelo C++
    with open(json_path, 'r', encoding='utf-8') as f:
        data = json.load(f)

    # Identifica o nó mais conectado (maior grau) para destacar visualmente
    degree = {}
    for edge in data.get('edges', []):
        degree[edge['from']] = degree.get(edge['from'], 0) + edge['value']
        degree[edge['to']] = degree.get(edge['to'], 0) + edge['value']

    # Configura os nós com tamanho proporcional ao grau e cores elegantes
    nodes = []
    for node in data.get('nodes', []):
        node_id = node['id']
        node_deg = degree.get(node_id, 1)
        
        # Diferencia termos comuns de termos de brainrot usando cores
        is_slang = node_id in ['sigma', 'aura', 'rizz', 'skibidi', 'gyatt', 'fanum', 'ohio', 'edging', 'mewing', 'looksmaxxing', 'brainrot', 'mog', 'npc']
        color = {
            'background': '#8A2BE2' if is_slang else '#4682B4',  # Roxo para brainrot, Azul para comum
            'border': '#4B0082' if is_slang else '#1F4E79',
            'highlight': {
                'background': '#DA70D6',
                'border': '#8B008B'
            }
        }
        
        nodes.append({
            'id': node_id,
            'label': node_id,
            'value': node_deg,
            'title': f"Termo: {node_id}<br>Grau de Conexão: {node_deg}",
            'color': color,
            'font': {'color': '#ffffff', 'size': 14, 'face': 'Courier New'}
        })

    # Configura as arestas (espessura baseada na coocorrência)
    edges = []
    for edge in data.get('edges', []):
        edges.append({
            'from': edge['from'],
            'to': edge['to'],
            'value': edge['value'],
            'title': f"Coocorrência: {edge['value']} documentos",
            'color': {'color': '#cccccc', 'highlight': '#ff5555'}
        })

    # Template HTML com Vis.js
    html_template = f"""<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Visualização do Grafo de Coocorrência Brainrot</title>
    <script type="text/javascript" src="https://unpkg.com/vis-network/standalone/umd/vis-network.min.js"></script>
    <style type="text/css">
        body {{
            font-family: 'Outfit', sans-serif;
            background-color: #0f0f1a;
            color: #ffffff;
            margin: 0;
            padding: 0;
            display: flex;
            flex-direction: column;
            align-items: center;
            height: 100vh;
        }}
        h1 {{
            margin-top: 20px;
            font-size: 2.2rem;
            color: #a855f7;
            text-shadow: 0 0 10px rgba(168, 85, 247, 0.5);
        }}
        .description {{
            font-size: 1.1rem;
            color: #94a3b8;
            margin-bottom: 20px;
            max-width: 800px;
            text-align: center;
        }}
        #mynetwork {{
            width: 90%;
            height: 70vh;
            border: 2px solid #3b0764;
            border-radius: 15px;
            background-color: #121224;
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.5);
        }}
        .legend {{
            display: flex;
            gap: 20px;
            margin-top: 15px;
            font-size: 0.95rem;
        }}
        .legend-item {{
            display: flex;
            align-items: center;
            gap: 8px;
        }}
        .box {{
            width: 15px;
            height: 15px;
            border-radius: 3px;
        }}
        .purple-box {{ background-color: #8A2BE2; }}
        .blue-box {{ background-color: #4682B4; }}
    </style>
</head>
<body>
    <h1>Rede de Coocorrência - Termos Brainrot</h1>
    <div class="description">
        Este grafo interativo representa a relação de coocorrência entre termos no corpus de documentos.
        Nós <b>roxos</b> representam gírias de brainrot, enquanto os <b>azuis</b> representam termos comuns aceitos.
        A espessura da linha (aresta) indica a frequência conjunta em que esses termos aparecem no mesmo documento.
    </div>
    
    <div id="mynetwork"></div>

    <div class="legend">
        <div class="legend-item">
            <div class="box purple-box"></div>
            <span>Gírias de Brainrot</span>
        </div>
        <div class="legend-item">
            <div class="box blue-box"></div>
            <span>Termos Comuns</span>
        </div>
    </div>

    <script type="text/javascript">
        const nodes = new vis.DataSet({json.dumps(nodes)});
        const edges = new vis.DataSet({json.dumps(edges)});

        const container = document.getElementById('mynetwork');
        const data = {{
            nodes: nodes,
            edges: edges
        }};
        const options = {{
            nodes: {{
                shape: 'dot',
                scaling: {{
                    min: 15,
                    max: 45
                }},
                borderWidth: 2,
                shadow: {{
                    enabled: true,
                    color: 'rgba(0,0,0,0.5)',
                    size: 10,
                    x: 5,
                    y: 5
                }}
            }},
            edges: {{
                scaling: {{
                    min: 1,
                    max: 8
                }},
                color: {{
                    color: '#64748b',
                    highlight: '#d946ef',
                    hover: '#d946ef',
                    inherit: false
                }},
                smooth: {{
                    type: 'continuous',
                    roundness: 0.5
                }}
            }},
            physics: {{
                stabilization: true,
                solver: 'forceAtlas2Based',
                forceAtlas2Based: {{
                    gravitationalConstant: -1000,
                    centralGravity: 0.001,
                    springLength: 500,
                    springConstant: 0.04,
                    damping: 0.4,
                    avoidOverlap: 1.0
                }}
            }},
            interaction: {{
                hover: true,
                tooltipDelay: 200
            }}
        }};
        const network = new vis.Network(container, data, options);
    </script>
</body>
</html>
"""

    with open(html_path, 'w', encoding='utf-8') as f:
        f.write(html_template)

    print(f"Sucesso! Visualização interativa gerada em '{html_path}'.")
    
    # Abre a página no navegador
    webbrowser.open('file://' + os.path.realpath(html_path))

if __name__ == '__main__':
    main()
