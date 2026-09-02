# Conjunto de Mandelbrot — Serial, OpenMP e Pthreads
 
Programa em C que gera o Conjunto de Mandelbrot utilizando quatro estratégias de
cálculo diferentes: **Serial**, **OpenMP** e duas implementações com **Pthreads**
(divisão em blocos contíguos e divisão round-robin). Todas as implementações
geram exatamente a mesma imagem.
 
Trabalho da disciplina de Infraestrutura de Software — Implementação 2.
 
## Requisitos
 
- Linux, Unix ou macOS
- `gcc` com suporte a OpenMP e Pthreads
- `make`
## Compilação
 
```bash
make
```
 
Isso gera o executável `mandelbrot` na raiz do projeto.
 
Para limpar os arquivos gerados (executável, imagens e times.txt):
 
```bash
make clean
```
 
## Execução
 
```bash
./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]
```
 
**Exemplo:**
 
```bash
./mandelbrot 800 600 1000 4
```
 
| Argumento        | Descrição                                             |
|-------------------|--------------------------------------------------------|
| `largura`         | Largura da imagem em pixels                            |
| `altura`           | Altura da imagem em pixels                              |
| `max_iteracoes`   | Número máximo de iterações por ponto                    |
| `num_threads`     | Número de threads usado nas implementações paralelas    |
 
Todos os argumentos devem ser números inteiros maiores que zero.
 
## Saída
 
A execução gera 5 arquivos no diretório corrente:
 
- `mandelbrot_agbo_serial.pgm` — implementação serial
- `mandelbrot_agbo_openmp.pgm` — implementação com OpenMP
- `mandelbrot_agbo_pthreads1.pgm` — Pthreads, divisão em blocos contíguos de linhas
- `mandelbrot_agbo_pthreads2.pgm` — Pthreads, divisão round-robin entre threads
- `times.txt` — tempo de execução (em segundos) de cada uma das quatro implementações
Os arquivos `.pgm` contêm apenas os valores de intensidade de cada pixel
(sem cabeçalho), um valor por pixel, separados por espaço, uma linha por
linha da imagem. As quatro imagens geradas são idênticas entre si.
 
O programa não imprime nada em `stdout` durante a execução normal. Erros
(número de argumentos incorreto, valores inválidos, falha de alocação de
memória ou de criação de arquivo/thread) são reportados e encerram a
execução.
 
## Estrutura do projeto
 
```
.
├── src/
│   └── main.c      # validação de argumentos, cálculo, saída dos arquivos
├── Makefile        # compilação (make) e limpeza (make clean)
└── README.md
```
 
## Autor
 
login: **agbo**