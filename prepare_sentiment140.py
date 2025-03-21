import csv
import random

def prepare_data(input_file, neutral_file, output_file):
    with open(input_file, 'r', encoding='latin-1') as csvfile:
        reader = csv.reader(csvfile)
        tweets = []
        for row in reader:
            polarity = int(row[0])
            text = row[5].replace('\n', ' ').strip().lower()
            neutral_indicators = ['meh', 'okay', 'not sure', 'whatever', 'idk', 'i guess', 'average', 'fine', 'so-so', 'alright', 'indifferent', 'neither', 'meh-ish']
            is_neutral = any(indicator in text for indicator in neutral_indicators)
            if is_neutral:
                label = '__label__neutral'
            elif polarity == 0:
                label = '__label__negative'
            elif polarity == 4:
                label = '__label__positive'
            else:
                label = '__label__neutral'
            tweets.append(f"{label} {text}")

    with open(neutral_file, 'r', encoding='utf-8') as f:
        neutral_tweets = f.readlines()
    tweets.extend(neutral_tweets)

    random.shuffle(tweets)
    with open(output_file, 'w', encoding='utf-8') as f:
        for tweet in tweets:
            f.write(tweet + '\n')

if __name__ == "__main__":
    prepare_data('training.1600000.processed.noemoticon.csv', 'neutral_tweets.txt', 'train.txt')