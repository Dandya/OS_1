#!/usr/bin/python3
# -*- coding: utf-8 -*-

from fastapi import FastAPI
import uvicorn

app = FastAPI()

@app.get("/index", tags=["Пример"])
def index():
    """ Пример простого вызова """
    return "Hello, world"

@app.get("/messages", tags=["Сообщения"])
def messages():
    """ Перечень доступных сообщений """
    return {
        "hello": "Формирует приведствие для кого-то",
        "bye": "Формирует прощание для кого-то",
    }

@app.get("/message/hello/{name}", tags=["Сообщения"])
def hello(name):
    """Формирует приведствие для кого-то"""
    return "Hello, " + name

@app.get("/message/bye/{name}", tags=["Сообщения"])
def bye(name):
    """Формирует прощание для кого-то"""
    return "Googbye, " + name


if __name__ == "__main__":
    uvicorn.run(app)
