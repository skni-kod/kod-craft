# Kod Craft

Kod craft jest klonem Minecrafta zrobionym w C++ z biblioteką raylib i integracją pytona.

## Kompilacja

### Nowa (wymaga perl'a)

Wywołaj w terminaly polecenie:

```cmd
perl ./bs.pl
```

Skrypt poprowadzi cię dalej.

### Stara

Stwórz folder `build` obok folderu `src`
w folderze build użyj poleceń:

gdy kompilujesz pierwszy raz: `cmake ..`

następnie `make`

to stworzy plik wykonywalny w folderze `build` o nazwie `kod-craft`