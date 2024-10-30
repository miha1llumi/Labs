with open("numbers.txt", "a") as file:
    for i in range(1, 101):
        file.write(f"{i}\n")
