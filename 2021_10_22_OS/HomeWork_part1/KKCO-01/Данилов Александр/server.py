import json
from typing import Optional
from fastapi import FastAPI
from fastapi.responses import PlainTextResponse, ORJSONResponse
import uvicorn
from os import listdir as dir

def createLibrary():
    """ create dictionary @library and save it in json file """
    try:
        allFiles=dir("./library")
    except:
        print("Error: dir not exist!")
        return None
    allWorks = []
    # scan files and save in allWorks
    for file in allFiles:
        if file.endswith(".lib"):
            allWorks.append(file)
    library = {}
    #create lists with author
    for works in allWorks:
        authors_work_lib = works.split(".")
        library[authors_work_lib[0]]=[]
    #add works in lists
    for work in allWorks:
        authors_work_lib = work.split(".")
        library[authors_work_lib[0]].extend([authors_work_lib[1]])
    libraryJSON=json.dumps(library)
    with open("./library/library.json", "w") as file:
        file.write(libraryJSON)
    return library


app = FastAPI()
library = {}
try:
    with open("library.json", "r") as file:
        JSON = file.read()
        library = json.loads(JSON)
except:
    library = createLibrary()
    if library == None:
        exit()

# Fucthions, which return json
@app.get("/jlibrary")
def Library():
    """ Список авторов: json """
    return {"items": list(library.keys())}

@app.get("/jlibrary/{author}")
def listOfWorks(author: str):
    """ Список работ: json """
    for authors in library.keys():
        if authors == author:
            return {"items": library[author]}
    return "404"

@app.get("/jlibrary/{author}/{work}")
def Work(author: str, work: str, begin: Optional[int]=1, end: Optional[int]=0):
    """ Работа автора: json """
    IsExistAuthor = False
    IsExistWork = False
    # scan author
    for authors in library.keys(): # all authors
        if authors == author:
            IsExistAuthor = True
            break;
    # scan work of author
    if IsExistAuthor == True:
        for works in library[author]: # all works of author
            if work == works:
                IsExistWork = True
                break;
    else:
        return "404"
    # return result
    if IsExistWork == True: # IsExistAuthor == True
        if begin < 1 or end < 0:
            return "Error in indexes: begin or end"
        workOfAuthor = '' 
        try:
            with open(f"./library/{author}.{work}.lib", "r") as file:
                workOfAuthor = file.read()
        except:
            return "404"
        Len = len(workOfAuthor)
        if end == 0 and begin <= Len:
            return  {"work": workOfAuthor[begin-1:]}
        elif end <= Len and begin <= Len and begin < end:
            return {"work": workOfAuthor[begin-1:end]}
        else:
            return "404"
    else:
        return "404"

# Fucthions, which return plain text
@app.get("/library", response_class=PlainTextResponse)
def Library():
    """ Список авторов """
    return "Список авторов:\n" + "\n".join(library.keys())

@app.get("/library/{author}", response_class=PlainTextResponse)
def listOfWorks(author: str):
    """ Список работ """
    for authors in library.keys():
        if authors == author:
            return "Список работ:\n" + "\n".join(library[author])
    return "404"

@app.get("/library/{author}/{work}", response_class=PlainTextResponse)
def Work(author: str, work: str, begin: Optional[int]=1, end: Optional[int]=0):
    """ Работа автора  """
    IsExistAuthor = False
    IsExistWork = False
    # scan author
    for authors in library.keys(): # all authors
        if authors == author:
            IsExistAuthor = True
            break;
    # scan work of author
    if IsExistAuthor == True:
        for works in library[author]: # all works of author
            if work == works:
                IsExistWork = True
                break;
    else:
        return "404"
    # return result
    if IsExistWork == True: # IsExistAuthor == True
        if begin < 1 or end < 0:
            return "Error in indexes: begin or end"
        workOfAuthor = '' 
        try:
            with open(f"./library/{author}.{work}.lib", "r") as file:
                workOfAuthor = file.read()
        except:
            return "404"
        Len = len(workOfAuthor)
        if end == 0 and begin <= Len:
            return workOfAuthor[begin-1:]
        elif end <= Len and begin <= Len and begin < end:
            return workOfAuthor[begin-1:end]
        else:
            return "Error in indexes: begin or end"
    else:
        return "404"

if __name__ == "__main__":
    uvicorn.run(app)

