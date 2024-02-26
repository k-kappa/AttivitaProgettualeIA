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

from asyncio import threads
import time
from PIL import Image, ImageTk
import tkinter as tk
from tkinter import ttk
import socket
import threading
from queue import Queue, Empty  # Importa Empty da queu

def start_server(stop_event, queue):
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

            # Metti i dati nella coda
            queue.put(data.decode('utf-8'))

            # Invia una risposta al client (se necessario)
            #response = "Ciao, ESP32! Ho ricevuto i tuoi dati."
            #server_socket.sendto(response.encode('utf-8'), client_address)

    except KeyboardInterrupt:
        # Gestisci l'interruzione da tastiera (es. Ctrl+C)
        print("Ricevuto KeyboardInterrupt. Chiudo l'applicazione.")
    finally:
        # Chiudi il socket
        server_socket.close()

class RotatedProgressBar(ttk.Progressbar):
    def __init__(self, master=None, **kw):
        ttk.Progressbar.__init__(self, master, **kw)
        self.rotation_angle = 0

    def rotate(self, angle):
        self.rotation_angle = angle
        self.redraw()

    def redraw(self):
        # Ottieni l'immagine associata alla barra di progresso
        image = self._style.tkraise('TProgressbar').element_create('image', 'image', self['value'])
        image_data = self._style.tkraise('TProgressbar').element_get('image', ('image',))
        image = ImageTk.PhotoImage(image_data['image'])

        # Ruota l'immagine
        rotated_image = image.rotate(self.rotation_angle)

        # Aggiorna l'immagine della barra di progresso
        self.configure(image=rotated_image)
        self.update()

class App:
    def __init__(self, root):
        self.root = root
        self.root.title("Visualizzatore Dati ESP32")

        # Aggiungi una variabile di bandiera per l'interruzione da tastiera
        self.stop_event = threading.Event()

        # Coda per passare i dati dal thread del server alla GUI
        self.queue = Queue()

        # Aggiungi la sezione per il testo utilizzando la widget Text
        self.text_widget = tk.Text(root, wrap=tk.WORD, height=10, width=40)
        self.text_widget.grid(row=0, column=0, padx=5, pady=5, sticky="nsew")  # Utilizzo sticky per allineare il testo verso il basso

        # Imposta uno scrollbar per consentire lo scrolling verticale
        scrollbar = ttk.Scrollbar(root, command=self.text_widget.yview)
        scrollbar.grid(row=0, column=1, padx=5, pady=5, sticky="ns")  # Utilizzo sticky per allineare la scrollbar verso il basso

        self.text_widget.config(yscrollcommand=scrollbar.set)

        root.grid_rowconfigure(0, weight=1)
        root.grid_columnconfigure(0, weight=1)

        # Avvia il server in un thread separato
        threading.Thread(target=start_server, args=(self.stop_event, self.queue), daemon=True).start()

        # Avvia la funzione di aggiornamento della GUI in un thread separato
        threading.Thread(target=self.update_gui_thread, daemon=True).start()

        # Aggiungi il gestore dell'interruzione da tastiera
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)


    def update_gui_thread(self):
        while not self.stop_event.is_set():
            try:
                data = self.queue.get_nowait()
                self.update_gui(data)
            except Empty:  # Usa Empty anziché queue.Empty
                pass
            time.sleep(0.1)

    def update_gui(self, data):
        # Aggiorna la GUI con i dati ricevuti
        # (Includi qui la logica per aggiornare gli elementi della GUI in base ai dati ricevuti)
        print(f"Dati ricevuti nella GUI: {data}")
        #self.aggiorna_progressbar(1,13,25,34,55,88)
        self.aggiorna_testo(data)

    def aggiorna_testo(self, new_text):
        # Aggiorna il testo nella sezione dedicata
        self.text_widget.insert(tk.END, new_text + "\n")
        # Auto-scroll verso il basso
        self.text_widget.see(tk.END)

    def on_close(self):
        # Imposta l'evento di interruzione
        self.stop_event.set()
        # Chiudi la finestra principale
        self.root.destroy()

# Crea l'applicazione
root = tk.Tk()
app = App(root)

root.geometry("400x300")

# Esegui l'applicazione
root.mainloop()
