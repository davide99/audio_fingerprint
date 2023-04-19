import matplotlib.pyplot as plt

if __name__ == '__main__':
    originals = []
    recs = []

    with open('data.csv', 'r') as f:
        for line in f:
            splitted = line.strip().split(",")
            original = int(splitted[0])
            rec = int(splitted[1])
            originals.append(original)
            recs.append(rec)

    plt.scatter(originals, recs, s=50, alpha=0.2, linewidth=0, facecolor='red')

    # Aggiunge etichette agli assi x e y
    plt.xlabel("Valori di x")
    plt.ylabel("Valori di y")

    # Aggiunge un titolo al grafico
    plt.title("Scatter plot di esempio")

    # Mostra lo scatter plot
    plt.show()
