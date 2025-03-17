import pandas as pd

# Load Sentiment140 CSV
df = pd.read_csv("/Users/rawad/Downloads/training.1600000.processed.noemoticon.csv", encoding="ISO-8859-1", 
                 names=["polarity", "id", "date", "query", "user", "text"])
df = df[["polarity", "text"]]
df["polarity"] = df["polarity"].replace({0: "__label__negative", 4: "__label__positive"})

# Sample 5,000 negative and 5,000 positive tweets for balance
df_negative = df[df["polarity"] == "__label__negative"].sample(n=5000, random_state=42)
df_positive = df[df["polarity"] == "__label__positive"].sample(n=5000, random_state=42)
df_balanced = pd.concat([df_negative, df_positive])

# Save to train.txt
with open("train.txt", "w", encoding="utf-8") as f:
    for index, row in df_balanced.iterrows():
        f.write(f"{row['polarity']} {row['text']}\n")

print("Training data prepared in train.txt")