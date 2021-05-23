import wikipedia
from random_word import RandomWords
from tkinter import *

def random_wiki(occurances):
    lst = []
    for i in range(occurances):
        r = RandomWords()
        word = r.get_random_word()
        page = wikipedia.page(word)
        texts = page.summary
        draw(word, texts)
        

def draw(word, texts):
    top = Tk()
    text = Text(top)
    text.insert(INSERT, str(texts))
    text.pack()
    top.title(str(word))
    top.mainloop()

random_wiki(2)
