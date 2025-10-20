import pandas as pd
import numpy as np

def create_flow_id(row):
    protocol = row['Protocol'].replace(' ', '_').replace('.', '_')
    source_ip = row['Source']
    destination_ip = row['Destination']
    
    return f"{source_ip}_{destination_ip}_{protocol}"

def filter_protocols(df):
    is_tcp = df['Protocol'].str.contains('TCP', case=False, na=False)
    is_icmp = df['Protocol'].str.contains('ICMP', case=False, na=False)
    
    df_filtered = df[is_tcp | is_icmp].copy()
    
    return df_filtered

def analyze_network_flows(df):
    df['flow_id'] = df.apply(create_flow_id, axis=1)
    
    df[['Source_IP', 'Destination_IP', 'Protocol']] = df['flow_id'].str.split('_', expand=True)
    
    df['Time'] = df['Time'].astype(float)
    df['Length'] = pd.to_numeric(df['Length'], errors='coerce')
    
    flow_metrics = []
    
    for flow in df['flow_id'].unique():
        flow_data = df[df['flow_id'] == flow].copy()
        
        flow_data = flow_data.sort_values('Time')
        
        time_diffs = flow_data['Time'].diff().dropna()
        
        avg_size = flow_data['Length'].mean()
        min_size = flow_data['Length'].min()
        max_size = flow_data['Length'].max()
        total_size = flow_data['Length'].sum()
        flow_duration = flow_data['Time'].max() - flow_data['Time'].min()
        avg_time_interval = time_diffs.mean() if not time_diffs.empty else 0
        min_time_interval = time_diffs.min() if not time_diffs.empty else 0
        max_time_interval = time_diffs.max() if not time_diffs.empty else 0
        
        avg_rate = (total_size * 8) / flow_duration if flow_duration > 0 else 0
        
        flow_metrics.append({
            'Protocolo': flow_data['Protocol'].iloc[0],
            'IP de Origem': flow_data['Source_IP'].iloc[0],
            'IP de Destino': flow_data['Destination_IP'].iloc[0],
            'tamanho medio dos pacotes': avg_size,
            'tamanho minimo dos pacotes': min_size,
            'tamanho maximo dos pacotes': max_size,
            'soma do tamanho dos pacotes': total_size,
            'duracao do fluxo': flow_duration,
            'media do intervalo de tempo': avg_time_interval,
            'intervalo minimo de tempo': min_time_interval,
            'intervalo maximo de tempo': max_time_interval,
            'taxa de media de bits por segundo': avg_rate
        })
    
    return pd.DataFrame(flow_metrics)

def process_wireshark_data(input_file, output_file):
    df = pd.read_csv(input_file)
    
    if 'No.' in df.columns:
        df = df.drop('No.', axis=1)
    
    df_filtered = filter_protocols(df)
    
    results = analyze_network_flows(df_filtered)
    
    results.to_csv(output_file, index=False)
    
    return results

if __name__ == "__main__":
    input_file = "wireshark.csv"
    output_file = "saida.csv"
    resultados = process_wireshark_data(input_file, output_file)
    if resultados is not None:
        print("Arquivo de saída gerado com sucesso")
