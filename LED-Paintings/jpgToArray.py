from PIL import Image

def ArrayConvert(filename):
    """
    Takes in 100x100px jpg or jpeg file.
    Scans through file by increments of 10px and appends to string stri.
    Outputs RGB values in C/Arduino Array format such that the array is
    defines as int painting_name[item_size][3] (item_size=100, 3=(R,G,B)).
    """
    image = Image.open(filename)
    arr = []
    for i in range(0,100,10):
        for j in range(100):
            color = image.getpixel((i,j))
            arr.append(color)
    stri = "{"
    count=0
    for i in range(0, len(arr), 10):
        count+=1
        arr[i] = str(arr[i])
        left = arr[i].find("(")
        right = arr[i].find(")")
        newlist = arr[i][left+1:right]
        final = newlist.split(", ")
        stri+="{"
        stri+= f"{final[0]},{final[1]},{final[2]}"
        stri+="},"
    stri = stri[:-1]
    stri+="}"
    return stri

if __name__ == "__main__":
    print(ArrayConvert("image_6.jpeg"))
