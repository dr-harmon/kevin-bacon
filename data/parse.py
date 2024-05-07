import csv

# Unzip with "gzip -d file.gz"
# title.basics: movie names
# name.basics: actor names
# title.principals: actor-movie relationships

# Exclude any movies older than this year
EARLIEST_YEAR = 1973

# Remove all columns from title.basics.tsv except ID and title
# Remove all rows that are 1) adult, or 2) not a movie, or 3) older than START_YEAR.
print("Parsing titles...")
newer_titles = set()
with open('title.basics.tsv', newline='') as infile, open('titles.tsv', 'w', newline='') as outfile:
    reader = csv.DictReader(infile, delimiter='\t')
    writer = csv.writer(outfile, delimiter='\t')
    for row in reader:
        try:
            startYear = int(row['startYear'])
        except ValueError:
            startYear = 0
        if row['titleType'] == "movie" and row['isAdult'] == "0" and startYear >= EARLIEST_YEAR:
            newer_titles.add(row['tconst'])
            writer.writerow([row['tconst'], row['primaryTitle']])

# Remove all columns from title.principals.tsv except tconst and nconst
print("Parsing principals...")
principal_names = set()
with open('title.principals.tsv', newline='') as infile, open('principals.tsv', 'w', newline='') as outfile:
    reader = csv.DictReader(infile, delimiter='\t')
    writer = csv.writer(outfile, delimiter='\t')
    for row in reader:
        if row['category'] == "actor" and row['tconst'] in newer_titles:
            principal_names.add(row['nconst'])
            writer.writerow([row['tconst'], row['nconst']])

# Remove all columns from name.basics.tsv except ID and name
# Remove all names from names.tsv not in principals.tsv
print("Parsing names...")
with open('name.basics.tsv', newline='') as infile, open('names.tsv', 'w', newline='') as outfile:
    reader = csv.DictReader(infile, delimiter='\t')
    writer = csv.writer(outfile, delimiter='\t')
    for row in reader:
        if row['nconst'] in principal_names:
            writer.writerow([row['nconst'], row['primaryName']])
