# convert_sentiment_lexicon.py

import requests
import re

# Read the input file from /Users/rawad/Documents/Projects/
input_path = "/Users/rawad/Documents/Projects/vader_lexicon.txt"
output_path = "/Users/rawad/Documents/CS3307B/MediaMinerSentiment/sentiment_lexicon.txt"

try:
    # Read the existing local file
    with open(input_path, "r", encoding="utf-8") as input_file:
        lines = input_file.readlines()

    with open(output_path, "w", encoding="utf-8") as output_file:
        for line in lines:
            line = line.strip()
            if not line or line.startswith("{") or line.startswith("std::"):  # Skip invalid or header lines
                continue
            # Match word followed by a number (score), ignoring extra columns
            match = re.match(r'^(\S+)\s+([-+]?\d*\.\d+|\d+)', line)
            if match:
                word = match.group(1).strip()
                score = match.group(2).strip()
                output_file.write(f"{word}\t{score}\n")
            else:
                print(f"Skipping unparseable line: {line}")

    print(f"Conversion complete. Saved to {output_path}")

except FileNotFoundError:
    print(f"Error: Input file not found at {input_path}")
    # Fallback: Download raw VADER lexicon if local file is missing
    url = "https://raw.githubusercontent.com/cjhutto/vaderSentiment/master/vaderSentiment/vader_lexicon.txt"
    response = requests.get(url)
    if response.status_code == 200:
        with open(output_path, "w", encoding="utf-8") as output_file:
            output_file.write(response.text)
        print(f"Fallback: Downloaded raw VADER lexicon to {output_path}")
    else:
        print(f"Error: Failed to download VADER lexicon. Status code: {response.status_code}")
except Exception as e:
    print(f"Error: {e}")