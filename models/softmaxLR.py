import m2cgen as m2c
import numpy as np
import pandas as pd
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, LabelEncoder


df = pd.read_csv("/home/user1/codingProjects/Fault_Detection_Fall2026/data/processed/stage4_features.csv")

cols_to_drop = ['scenario_id']
df.drop(columns=cols_to_drop, errors='ignore', inplace=True)

X = df[['send_v_rms_a_v', 'send_v_rms_b_v', 'send_v_rms_c_v', 
      'send_v1_rms_v', 'send_v2_rms_v', 'send_v0_rms_v', 
      'send_i_rms_a_a', 'send_i_rms_b_a', 'send_i_rms_c_a', 
      'send_i1_rms_a', 'send_i2_rms_a', 'send_z_magnitude_c_ohm', 
      'send_z_magnitude_a_ohm', 'send_z_magnitude_b_ohm', 
      'send_i0_rms_a', 'send_i_rms_imbalance_pct', 
      'send_v_rms_imbalance_pct', 'send_i2_i1_ratio', 
      'send_i0_i1_ratio', 'send_v2_v1_ratio', 
      'send_v0_v1_ratio']]


y = df['class_name']


train = df["split"].eq("train")
validation = df["split"].eq("validation")
test = df["split"].eq("test")

X_train, y_train = X.loc[train], y.loc[train]
X_val, y_val = X.loc[validation], y.loc[validation]
X_test, y_test = X.loc[test], y.loc[test]


scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_val_scaled = scaler.transform(X_val)
X_test_scaled = scaler.transform(X_test)

model = LogisticRegression(solver='lbfgs', max_iter=200)
model.fit(X_train_scaled, y_train)

# 2. Transpile the model to pure C code
c_code = m2c.export_to_c(model)

# 3. Save as a header file for the ESP32
with open("model.h", "w") as f:
    f.write(c_code)

print("Model exported to model.h successfully!")
