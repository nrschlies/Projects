"""
Written by: Noah Schliesman
Utilizes wikipedia and random python libraries to generate random word summaries
"""
import wikipedia
from random_word import RandomWords
from tkinter import *

#Fuction that generates random wikipedia page
def random_wiki(occurances):
    lst = []
    #Occurances is an input of how many articles you want
    for i in range(occurances):
        r = RandomWords()
        word = r.get_random_word()
        page = wikipedia.page(word)
        texts = page.summary
        draw(word, texts)
        
# Function that creates a tkinter box and populates it with wikipedia data
def draw(word, texts):
    top = Tk()
    text = Text(top)
    text.insert(INSERT, str(texts))
    text.pack()
    top.title(str(word))
    top.mainloop()

#call to function
random_wiki(2)
