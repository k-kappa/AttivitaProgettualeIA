'''
import socket

def start_server():
    # Ottieni l'indirizzo IP locale del server
    host = socket.gethostbyname(socket.gethostname())

    # Porta sulla quale ascoltare le connessioni UDP
    port = 2025  # Scegli una porta disponibile

    # Creazione del socket UDP
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Associazione del socket all'indirizzo IP e alla porta
    server_socket.bind((host, port))

    print(f"Server in ascolto su {host}:{port}")

    try:
        while True:
            # Ricevi i dati dal client
            data, client_address = server_socket.recvfrom(1024)
            print(f"Dati ricevuti da {client_address}: {data.decode('utf-8')}")

            # Invia una risposta al client (se necessario)
            response = "Ciao, ESP32! Ho ricevuto i tuoi dati."
            server_socket.sendto(response.encode('utf-8'), client_address)

    except KeyboardInterrupt:
        # Gestisci l'interruzione da tastiera (es. Ctrl+C)
        print("Ricevuto KeyboardInterrupt. Chiudo l'applicazione.")
        server_socket.close()

if __name__ == "__main__":
    start_server()
'''
'''
import tkinter as tk
from tkinter import ttk
import socket
import threading

def start_server():
    # Ottieni l'indirizzo IP locale del server
    host = socket.gethostbyname(socket.gethostname())

    # Porta sulla quale ascoltare le connessioni UDP
    port = 2025  # Scegli una porta disponibile

    # Creazione del socket UDP
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Associazione del socket all'indirizzo IP e alla porta
    server_socket.bind((host, port))

    print(f"Server in ascolto su {host}:{port}")

    try:
        while True:
            # Ricevi i dati dal client
            data, client_address = server_socket.recvfrom(1024)
            print(f"Dati ricevuti da {client_address}: {data.decode('utf-8')}")

            # Invia una risposta al client (se necessario)
            response = "Ciao, ESP32! Ho ricevuto i tuoi dati."
            server_socket.sendto(response.encode('utf-8'), client_address)

    except KeyboardInterrupt:
        # Gestisci l'interruzione da tastiera (es. Ctrl+C)
        print("Ricevuto KeyboardInterrupt. Chiudo l'applicazione.")
        server_socket.close()

class App:
    def __init__(self, root):
        self.root = root
        self.root.title("Visualizzatore Dati ESP32")

        # Aggiungi i pulsanti
        self.button1 = tk.Button(root, text="Pulsante 1", command=self.azione_pulsante1)
        self.button1.pack()

        self.button2 = tk.Button(root, text="Pulsante 2", command=self.azione_pulsante2)
        self.button2.pack()

        self.button3 = tk.Button(root, text="Pulsante 3", command=self.azione_pulsante3)
        self.button3.pack()

        self.button4 = tk.Button(root, text="Pulsante 4", command=self.azione_pulsante4)
        self.button4.pack()

        # Aggiungi la sezione per gli indicatori di livello verticali
        self.progressbar_frame = ttk.Frame(root)
        self.progressbar_frame.pack()

        self.progressbar1 = ttk.Progressbar(self.progressbar_frame, length=50, mode="determinate", orient="vertical")
        self.progressbar1.grid(row=0, column=0, padx=5, pady=5)

        self.progressbar2 = ttk.Progressbar(self.progressbar_frame, length=50, mode="determinate", orient="vertical")
        self.progressbar2.grid(row=0, column=1, padx=5, pady=5)

        self.progressbar3 = ttk.Progressbar(self.progressbar_frame, length=50, mode="determinate", orient="vertical")
        self.progressbar3.grid(row=0, column=2, padx=5, pady=5)

        self.progressbar_frame2 = ttk.Frame(root)
        self.progressbar_frame2.pack()

        self.progressbar4 = ttk.Progressbar(self.progressbar_frame2, length=50, mode="determinate", orient="vertical")
        self.progressbar4.grid(row=0, column=0, padx=5, pady=5)

        self.progressbar5 = ttk.Progressbar(self.progressbar_frame2, length=50, mode="determinate", orient="vertical")
        self.progressbar5.grid(row=0, column=1, padx=5, pady=5)

        self.progressbar6 = ttk.Progressbar(self.progressbar_frame2, length=50, mode="determinate", orient="vertical")
        self.progressbar6.grid(row=0, column=2, padx=5, pady=5)

        # Aggiungi la sezione per il testo
        self.text_label = tk.Label(root, text="Testo di esempio")
        self.text_label.pack()

        # Avvia il server in un thread separato
        threading.Thread(target=self.start_server_thread).start()

        self.root.protocol("WM_DELETE_WINDOW", self.on_close)

    def azione_pulsante1(self):
        # Implementa l'azione del pulsante 1
        pass

    def azione_pulsante2(self):
        # Implementa l'azione del pulsante 2
        pass

    def azione_pulsante3(self):
        # Implementa l'azione del pulsante 3
        pass

    def azione_pulsante4(self):
        # Implementa l'azione del pulsante 4
        pass

    def aggiorna_progressbar(self, value1, value2, value3, value4, value5, value6):
        # Aggiorna gli indicatori di livello con i valori forniti
        self.progressbar1["value"] = value1
        self.progressbar2["value"] = value2
        self.progressbar3["value"] = value3
        self.progressbar4["value"] = value4
        self.progressbar5["value"] = value5
        self.progressbar6["value"] = value6

    def aggiorna_testo(self, new_text):
        # Aggiorna il testo nella sezione dedicata
        self.text_label.config(text=new_text)

    def on_close(self):
        # Imposta la variabile di bandiera per l'interruzione da tastiera
        self.interrupted = True
        # Chiudi la finestra principale
        self.root.destroy()

    def start_server_thread(self):
        start_server()

# Crea l'applicazione
root = tk.Tk()
app = App(root)

# Esegui l'applicazione
root.mainloop()
'''
import tkinter as tk
from tkinter import ttk
import socket
import threading
import time

def start_server(stop_event):
    # Ottieni l'indirizzo IP locale del server
    host = socket.gethostbyname(socket.gethostname())

    # Porta sulla quale ascoltare le connessioni UDP
    port = 2025  # Scegli una porta disponibile

    # Creazione del socket UDP
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Associazione del socket all'indirizzo IP e alla porta
    server_socket.bind((host, port))

    print(f"Server in ascolto su {host}:{port}")

    try:
        while not stop_event.is_set():
            # Ricevi i dati dal client
            data, client_address = server_socket.recvfrom(1024)
            print(f"Dati ricevuti da {client_address}: {data.decode('utf-8')}")

            # Invia una risposta al client (se necessario)
            response = "Ciao, ESP32! Ho ricevuto i tuoi dati."
            server_socket.sendto(response.encode('utf-8'), client_address)

    except KeyboardInterrupt:
        # Gestisci l'interruzione da tastiera (es. Ctrl+C)
        print("Ricevuto KeyboardInterrupt. Chiudo l'applicazione.")
    finally:
        # Chiudi il socket
        server_socket.close()

def start_server_thread(stop_event):
    while not stop_event.is_set():
        start_server(stop_event)

class App:
    def __init__(self, root):
        self.root = root
        self.root.title("Visualizzatore Dati ESP32")

        # Aggiungi una variabile di bandiera per l'interruzione da tastiera
        self.stop_event = threading.Event()

        # Aggiungi i pulsanti
        self.button1 = tk.Button(root, text="Pulsante 1", command=self.azione_pulsante1)
        self.button1.pack()

        self.button2 = tk.Button(root, text="Pulsante 2", command=self.azione_pulsante2)
        self.button2.pack()

        self.button3 = tk.Button(root, text="Pulsante 3", command=self.azione_pulsante3)
        self.button3.pack()

        self.button4 = tk.Button(root, text="Pulsante 4", command=self.azione_pulsante4)
        self.button4.pack()

        # Aggiungi la sezione per gli indicatori di livello verticali
        self.progressbar_frame = ttk.Frame(root)
        self.progressbar_frame.pack()

        self.progressbar1 = ttk.Progressbar(self.progressbar_frame, length=50, mode="determinate", orient="vertical")
        self.progressbar1.grid(row=0, column=0, padx=5, pady=5)

        self.progressbar2 = ttk.Progressbar(self.progressbar_frame, length=50, mode="determinate", orient="vertical")
        self.progressbar2.grid(row=0, column=1, padx=5, pady=5)

        self.progressbar3 = ttk.Progressbar(self.progressbar_frame, length=50, mode="determinate", orient="vertical")
        self.progressbar3.grid(row=0, column=2, padx=5, pady=5)

        self.progressbar_frame2 = ttk.Frame(root)
        self.progressbar_frame2.pack()

        self.progressbar4 = ttk.Progressbar(self.progressbar_frame2, length=50, mode="determinate", orient="vertical")
        self.progressbar4.grid(row=0, column=0, padx=5, pady=5)

        self.progressbar5 = ttk.Progressbar(self.progressbar_frame2, length=50, mode="determinate", orient="vertical")
        self.progressbar5.grid(row=0, column=1, padx=5, pady=5)

        self.progressbar6 = ttk.Progressbar(self.progressbar_frame2, length=50, mode="determinate", orient="vertical")
        self.progressbar6.grid(row=0, column=2, padx=5, pady=5)

        # Aggiungi la sezione per il testo
        self.text_label = tk.Label(root, text="Testo di esempio")
        self.text_label.pack()

        # Avvia il server in un thread separato
        threading.Thread(target=start_server_thread, args=(self.stop_event,), daemon=True).start()

        # Aggiungi il gestore dell'interruzione da tastiera
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)

    def azione_pulsante1(self):
        # Implementa l'azione del pulsante 1
        pass

    def azione_pulsante2(self):
        # Implementa l'azione del pulsante 2
        pass

    def azione_pulsante3(self):
        # Implementa l'azione del pulsante 3
        pass

    def azione_pulsante4(self):
        # Implementa l'azione del pulsante 4
        pass

    def aggiorna_progressbar(self, value1, value2, value3, value4, value5, value6):
        # Aggiorna gli indicatori di livello con i valori forniti
        self.progressbar1["value"] = value1
        self.progressbar2["value"] = value2
        self.progressbar3["value"] = value3
        self.progressbar4["value"] = value4
        self.progressbar5["value"] = value5
        self.progressbar6["value"] = value6

    def aggiorna_testo(self, new_text):
        # Aggiorna il testo nella sezione dedicata
        self.text_label.config(text=new_text)

    def on_close(self):
        # Imposta l'evento di interruzione
        self.stop_event.set()
        # Chiudi la finestra principale
        self.root.destroy()

# Crea l'applicazione
root = tk.Tk()
app = App(root)

# Esegui l'applicazione
root.mainloop()
