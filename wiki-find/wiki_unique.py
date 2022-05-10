"""
Written by: Noah Schliesman
Modified version of wiki_sort to only pick from unusual articles.
"""

import wikipedia
import random
from tkinter import *

def initialize_wiki(): 
    """
    Checks for links in unusual articles wikipedia page. Choses a random 
    word from those links, and provides a summary of that word.
    Returns a tuple of the word and the wikipedia summary of that word.
    """
    article = wikipedia.page("Wikipedia:Unusual articles") 
    unusual = article.links
    word = random.choices(unusual, k=1) 
    word = "".join(word) #change word to string
    
    return (word, wikipedia.summary(word)) 

def initialize_box(word, summary):
    """
    Creates tkinter window with word and summary.
    """
    top = Tk()
    text = Text(top)
    text.insert(INSERT, str(summary))
    text.pack()
    top.title(str(word))
    top.mainloop()

if __name__ == '__main__':
    (word, summary) = initialize_wiki()
    initialize_box(word, summary)
