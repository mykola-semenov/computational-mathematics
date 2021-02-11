import gc

import pandas as pd
import numpy as np
import pickle as pkl
import h5py

from sklearn.model_selection import train_test_split
import torch


def XY_split(dataset, OW, PH, PW):
    dataset.sort_index(inplace=True)
    X, Y = [], []
    for i in range(OW):
        t = dataset.iloc[i:-(PH + PW + OW - 1 - i)]
        X.append(pd.DataFrame(t.index.values.reshape(-1, 1), columns=[f't_{i}']))
        X.append(pd.DataFrame(t.values, columns=t.columns))
    X = pd.concat(X, axis=1)
    for i in range(OW + PH, OW + PH + PW):
        t = dataset.iloc[i:] if i == OW + PH + PW - 1 else dataset.iloc[i:-(OW + PH + PW - 1 - i)]
        Y.append(pd.DataFrame(t.values, columns=t.columns))
        X[f't_{i}'] = t.index.values
    Y = pd.concat(Y, axis=1)
    gc.collect()
    return X, Y


def train_eval_split(dataset, eval_size, OW, PH, PW):
    dataset.sort_index(inplace=True)
    # eval_size ratio correction
    size = dataset.shape[0]
    if type(eval_size) is float:
        eval_size += (1 - 2 * eval_size) / size * (OW + PH + PW - 1)
        eval_size = int(eval_size * size)
    gc.collect()
    return dataset.iloc[:-eval_size], dataset.iloc[-eval_size:]


def prepare(dataset, eval_size, OW, PH, PW):
    dataset.sort_index(inplace=True)
    data_train, data_eval = train_eval_split(dataset, eval_size, OW, PH, PW)
    train_X, train_Y = XY_split(data_train, OW, PH, PW)
    eval_X, eval_Y = XY_split(data_eval, OW, PH, PW)
    gc.collect()
    return train_X, train_Y, eval_X, eval_Y


def load(pkl_path, h5_path, locations_path, distances_path):
    sensor_ids, sensor2node, adjacency_matrix = pd.read_pickle(pkl_path)
    data = h5py.File(h5_path, 'r')
    locations = pd.read_csv(locations_path, header=None, names=['sensor_id', 'x', 'y'])
    distances = pd.read_csv(distances_path, header=None, names=['from', 'to', 'distance'])

    distances['from'] = distances['from'].apply(list(map(int, sensor_ids)).index)
    distances['to'] = distances['to'].apply(list(map(int, sensor_ids)).index)
    locations['sensor_id'] = locations['sensor_id'].apply(list(map(int, sensor_ids)).index)
    locations.set_index(['sensor_id'], inplace=True)
    locations.sort_index(inplace=True)
    speed_data = pd.DataFrame(
        data['speed']['block0_values'][()],
        index=pd.to_datetime(data['speed']['axis1'][()]),
        columns=list(map(list(map(int, sensor_ids)).index, data['speed']['axis0'][()]))
    )
    gc.collect()
    return adjacency_matrix, speed_data, locations, distances


def tensor_split(speed_data, adjacency, distances, OW, PH, PW):
    speed_data = speed_data.sort_index()
    T_O, V_O, T_P, V_P = [], [], [], []
    for i in range(OW):
        t = speed_data.iloc[i:-(PH + PW + OW - 1 - i)]
        T_O.append(t.index.values.astype(np.int) // 1_000_000_000 // 300)
        V_O.append(t.values)
    for i in range(OW + PH, OW + PH + PW):
        t = speed_data.iloc[i:] if i == OW + PH + PW - 1 else speed_data.iloc[i:-(OW + PH + PW - 1 - i)]
        T_P.append(t.index.values.astype(np.int) // 1_000_000_000 // 300)
        V_P.append(t.values)
    T_O = torch.LongTensor(np.stack(T_O, axis=1))
    V_O = torch.Tensor(np.stack(V_O, axis=1))
    T_P = torch.LongTensor(np.stack(T_P, axis=1))
    V_P = torch.Tensor(np.stack(V_P, axis=1))
    A = torch.Tensor(adjacency)
    
    gc.collect()
    return T_O, V_O, T_P, V_P, A


def prepare_tensors(speed_data, adjacency, distances, eval_size, OW, PH, PW):
    speed_data = speed_data.sort_index()
    speed_data_train, speed_data_eval = train_eval_split(speed_data, eval_size, OW, PH, PW)
    gc.collect()
    return tensor_split(speed_data_train, adjacency, distances, OW, PH, PW), tensor_split(speed_data_eval, adjacency, distances, OW, PH, PW)