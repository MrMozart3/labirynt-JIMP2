# Labsolver

---

## Zasady działania

**Labsolver** jest programem napisanym w języku C, służącym do rozwiązywania labiryntów z ograniczeniem pamięci. Aby ograniczyć zużycie pamięci, labirynt dzielony jest na mniejsze elementy zwane "chunkami". Program przejmuje labirynt w formie tekstowej i rozwiązuje go, korzystając z dynamicznego przydziału pamięci.

### Przykład użycia

Aby uruchomić program, należy postępować zgodnie z poniższymi krokami:

1. Pobierz pliki programu.
2. Umieść labirynt w formacie opisanym w sekcji "Format przekazywanego labiryntu" w katalogu z pobranymi plikami.
3. Uruchom plik makefile.
4. W terminalu wpisz: `./labsolver` z odpowiednimi parametrami.

Przykłady użycia:
```bash
./labsolver -l labirynt.txt -f rozwiazanie.txt
./labsolver -l labirynt.txt -f rozwiazanie.txt -d
'''

Więcej informacji na temat parametrów wejściowych znajdziesz w sekcji "Parametry wejściowe".

---

## Dane wejściowe i wyjściowe

Labirynt jest reprezentowany w formie tekstowej, gdzie:
- `P` oznacza punkt wejścia,
- `K` oznacza punkt wyjścia,
- `X` oznacza ścianę labiryntu,
- spacja oznacza miejsce, po którym można się poruszać.

### Obsługa błędów

W przypadku błędów, informacje są zapisywane do pliku `log.txt`. Oto znaczenia kodów błędów:

- `11` - Zły format labiryntu
- `12` - Błąd zapisu pliku wyjściowego
- `13` - Niepoprawny parametr wejściowy

Więcej informacji na temat kodów błędów znajdziesz w dokumentacji.

---

## Instrukcja HELP

Aby uzyskać pomoc, wpisz:
'./labsolver -h'


