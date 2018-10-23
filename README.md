# SocialNetwork
School project. Generates all subsets of dissimilar users given some input.

## What does this do?
When run with input.txt as an argument, dis.cpp parses a file (with a given format) containing the number of users, the number of content items, the friendship relations, ownership relations, and 'like' relations. All friends like each other's content, and if you like the content that someone owns, then you're considered similar. dis.cpp uses adjacency matrices to find all dissimilar sets of users and prints them to stdout. This involves iterating through all possible subsets of users (using bit operations).
