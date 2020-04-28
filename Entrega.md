# Simulador de Propagação de Vírus com OpenMP

Nome: Emílio B. Pedrollo

## Desenvolvimento

### Código
    
O [código] para o trabalho foi desenvolvido em cima do existente usando definições de compilação
para gerar binários com comportamentos distintos (veja [Makefile] e/ou [CMakeLists.txt])

### Binários

É possível gerar 4 executáveis com comportamentos distintos:

 * **virusim**: Comportamento original inalterado;
 * **virusim_parallel_probs**: Paralelizando o loop de probabilidades;
 * **virusim_parallel_trials**: Paralelizando o loop de amostragens;
 * **virusim_parallel_probs_n_trials**: Paralelizando ambos os loops de probabilidades e amostragens;
 
### Compilando

É possível compilar o código utilizando tanto o padrão **make** para Unix quando o multiplataforma **cmake**

#### make

Dentro do diretório `virusim` execute:

```bash
$ make
```

Após a execução os binários estarão na pasta `output`.

#### cmake

Na raiz do projeto execute:

```bash
$ cmake .
$ cmake --build . --config Release
```

Após a execução os binários estarão na pasta `output/<BuildType>` onde `<BuildType>` corresponde ao tipo de 
build definido por `--config`, se esse valor é `Debug` por padrão se não for informado.
 
## Resultados
 
Uma tabela com os tempos, aceleração e eficiencia para diversas configurações de numero de threads 
e tamanhos de problemas para _virusim_parallel_probs_ e _virusim_parallel_probs_ pode ser encontrado
em [Results.md]. 

## Referências

 * https://www.openmp.org/wp-content/uploads/openmp-examples-4.5.0.pdf
 * https://openmp.org/wp-content/uploads/sc13.tasking.ruud.pdf
 
[código]: virusim/virusim.cpp
[Makefile]: virusim/Makefile
[CMakeLists.txt]: CMakeLists.txt
[Results.md]: RESULTS.md