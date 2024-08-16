# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

*
Matteo Ginhson: [Realizo gran parte del codigo encargado de mover los cuerpos y los asteroides]
Nicanor Otamendi: [Realizo la mayor parte del codigo visual y la creacion de asteroides]

[De todas formas al ser un grupo de solo 2 personas, ambos trabajamos sobre todas las partes del codigo, en su mayoria de forma sincronica mediante liveshare.]

## Verificación del timestep

[Al notar que las derivadas son discretas, es decir no estamos derivanrdo continuamente, llegamos a la conclusion de que para lograr una mayor precision es necesario minimizar el intervalo para el cual se analiza, es decir reducir el timestep. El gran problema de esto es la utilizacion de recursos.]

## Verificación del tipo de datos float

[Luego de evaluar la precision que brindaba este tipo de datos y los recursos de las computadoras, consideramos mejor trabajar con tipo de dato double float. De esta forma, aumenta la precision de la simulacion reduciendo el impacto de los errores de manera significativa cuando pasa mucho tiempo dentro de la simulacion.]

## Complejidad computacional con asteroides

[completar]

## Mejora de la complejidad computacional

[completar]

## Bonus points

[Lo primero que hicimos con el fin de mejorar la precision fue reducir el timestep a 1 dia(s). Al hacerlo observamos que si bien la simulacion es mas exacta, esto consume demasiados recursos.]
