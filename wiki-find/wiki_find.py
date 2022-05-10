"""
Written by: Noah Schliesman
Utilizes wikipedia, random, and tkinter libraries to generate random word summaries.
"""
import wikipedia
from random_word import RandomWords
from tkinter import *

def random_wiki(occurances):
    """
    Generates random word and searches wikipedia for that word.
    Input is occurances (how many articles you want).
    Calls draw() which takes in the word and wikipedia summary and
    puts that into a tkinter window.
    """
    lst = []
    for i in range(occurances):
        r = RandomWords()
        word = r.get_random_word()
        page = wikipedia.page(word)
        texts = page.summary
        draw(word, texts)
        
def draw(word, texts):
    """
    Takes in wikipedia word and summary text and outputs into
    a tkinter window.
    """
    top = Tk()
    text = Text(top)
    text.insert(INSERT, str(texts))
    text.pack()
    top.title(str(word))
    top.mainloop()

if __name__ = "__main__":
    random_wiki(2) #Example: get 2 random summaries
