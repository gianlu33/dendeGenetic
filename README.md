# dendeGenetic
### Esecuzione del programma

Il programma va eseguito da command line **nella cartella in cui si trova**.

- Due modalità di funzionamento:

  1. `dendeGenetic.exe <outputfile>`

     Fa partire una nuova esecuzione del programma. La popolazione iniziale verrà generata casualmente. Alla conclusione (forzata), salva i risultati trovati fino a quel momento nel file `outputfile`

  2. `dendeGenetic.exe <inputfile> <outputfile>`

     "Riprende" il programma da dove si era interrotto. Legge i dati dal file `inputfile` (che deve essere un file generato in una precedente esecuzione, come output), e al termine dell'esecuzione salva i risultati, come prima, su `outputfile`

### Spiegazione dell'algoritmo

L'algoritmo genetico lavora su un set di soluzioni (chiamata "popolazione"), e sull'evoluzione di esso attraverso "generazioni". A ogni generazione, vengono create delle soluzioni derivate dalla popolazione della generazione precedente (i cosiddetti "children"). La creazione di un figlio (child) avviene mischiando i "geni" di due elementi della popolazione, estratti secondo un algoritmo. Una volta che la popolazione derivata è stata creata, viene attuato un processo di miglioramento di essa, chiamato "local search". Per ogni figlio generato, si vanno a ispezionare tutte le soluzioni vicine a esso, in modo da trovare una configurazione di geni migliore. Se questa configurazione è trovata, allora il figlio "muta" verso di essa.

### Esecuzione standard

L'algoritmo esegue i seguenti passi:

1. Inizializzazione (creazione della popolazione iniziale)

   Questo processo può essere evitato se viene indicato un file di input come visto sopra.

2. Creazione della nuova popolazione, "children" tramite combinazione degli elementi della vecchia generazione
3. Run dell'analisi per calcolare la bontà di ogni child.
4. Local search su ogni child.
5. La popolazione generata rimpiazza la precedente
6. Ritorno al punto 2. per una nuova generazione

### Terminazione del programma

L'esecuzione del programma non termina mai, senza intervento dell'utente andrebbe avanti all'infinito.

Per terminare il programma basta premere `CTRL+C` sulla linea di comando.

Dopo aver fatto ciò, il file di output verrà generato, in cui sarà salvata la popolazione dell'ultima generazione portata a termine. Inoltre, se nella generazione in corso è stata trovata una soluzione migliore di tutte quelle trovate fino a quel momento, anche questa sarà salvata nel file di output.

**NB** Il programma a volte non termina correttamente: infatti, può succedere che dopo aver premuto `CTRL+C` il programma dendeGenetic termini, ma eventuali analisi che erano in corso non vengano stoppate correttamente. Per accorgersi di ciò, basta monitorare i processi in esecuzione in `Gestione Attività` di Windows. Se la CPU ha una percentuale elevata anche dopo aver stoppato il programma, allora bisogna chiudere le simulazioni ancora in corso manualmente (dal task manager, fare "Termina Attività" nel processo opportuno).

Inoltre, successivamente bisogna rimuovere anche eventuali cartelle create nella cartella in cui si è eseguito il programma. Tali cartelle hanno il nome `__folderX__` e vanno eliminate DOPO aver terminato tutti i processi in corso. Questo va fatto necessariamente prima di avviare una nuova run, altrimento potrebbe non funzionare correttamente. 

Se le simulazioni sono stoppate tutte correttamente dopo aver premuto `CTRL+C`, queste cartelle vengono eliminate automaticamente.

Se non si riescono a chiudere le simulazioni ancora in corso, è sufficiente fare il logout da Windows oppure riavviare il pc.

### Verificare se una soluzione è effettivamente buona

Se si vuole verificare che una soluzione sia effettivamente buona, si può far partire un'analisi solo per essa nel seguente modo.

1. Aprire un terminale cmd nella cartella base del programma.

2. Creare una nuova cartella e chiamarla a piacere (es `prova`)

3. Digitare il seguente comando:

    `opensees 5MainFRPGenDende.tcl prova 0 1 0 1 ... 1 0`

   (le cifre 0/1 indicano la soluzione che si vuole testare)

4. Verrà fatta partire un'analisi su quella soluzione, e verranno generati nella cartella `prova` tutti i file che normalmente vengono creati con un'analisi tradizionale, in modo da poter verificare manualmente la bontà di quella soluzione.

### Calcolare la bontà di una soluzione a partire dai file generati

Se, a seguito di una simulazione manuale del file tcl, si vuole calcolare la bontà di una soluzione, è sufficiente usare il programma `dendeCalculator.exe`

**NB** Questo funziona soltanto se i file riguardanti il drift generati hanno nomi `1.txt`, `2.txt` e così via. La simulazione del paragrafo precedente genera i file con tali nomi, quindi è possibile calcolare la bontà di una soluzione generata in quel modo senza modificare niente. In alternativa, si possono rinominare manualmente i file.

Utilizzo:

`dendeCalculator.exe <folderName>` 

I file devono trovarsi nella cartella di nome folderName.

**NB** Al momento, dendeCalculator calcola la radice quadrata della somma dei quadrati dei massimi valori di ogni file.