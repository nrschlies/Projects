"""
Written by: Noah Schliesman
Modified version of wiki_sort to only pick from unusual articles
TO DO:
- Differentiate between title link and sub-link
- Make tkinter box prettier
"""

import wikipedia
import random
from tkinter import *

def initialize_wiki(): 
    #Checks for links in Unusual articles wikipedia page --> list
    h = wikipedia.page("Wikipedia:Unusual articles") 
    unusual = h.links

    word = random.choices(unusual, k=1) #chooses one random word from those links
    word = "".join(word) #change to string
    return (word, wikipedia.summary(word)) #summarizes word

#builds tkinter box
def initialize_box(word, summary):
    top = Tk()
    text = Text(top)
    text.insert(INSERT, str(summary))
    text.pack()
    top.title(str(word))
    top.mainloop()

def main():
    (word, summary) = initialize_wiki()
    initialize_box(word, summary)

if __name__ == '__main__':
    main()
