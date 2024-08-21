# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

*
Matteo Ginhson: [Realizo gran parte del codigo encargado de mover los cuerpos y los asteroides]
Nicanor Otamendi: [Realizo la mayor parte del codigo visual y la creacion de asteroides]

[De todas formas al ser un grupo de solo 2 personas, ambos trabajamos sobre todas las partes del codigo, en su mayoria de forma sincronica mediante Liveshare y GitHub.]

## Verificación del timestep

[Si bien al ser las derivadas discretas, reducir el timestep mejoraría la precisión, mas aumentarlo mejoraría la velocidad de la simulación. Intentamos aumentarlo lo máximo posible, pero notamos que con valores rondando los 500 * SECONDS_PER_DAY, la simulación era muy inexacta (Mercurio salia eyectado por el Sol en vez de mantenerse en su órbita). Así que preferimos mantener la fidelidad de la simulación ejecutando 100 días por UpdateOrbitalSimOptimized (El valor por defecto). Al testear este timestep, la simulación sigue siendo suficientemente precisa luego de mucho más que 100 años, lo cúal es suficiente para nuestros fines.]

## Verificación del tipo de datos float

[Luego de evaluar la precision que brindaba este tipo de datos y los recursos de las computadoras, consideramos mejor trabajar con tipo de dato double . De esta forma, aumenta la precision de la simulacion reduciendo el impacto de los errores de manera significativa cuando pasa mucho tiempo dentro de la simulacion, sumado al hecho de que nos sobran ampliamente los recursos para hacer este cambio.]

## Complejidad computacional con asteroides

[Al buscar el mayor realismo posible, se calcula la interacción de todos los cuerpos contra todos, teniendo en total n elementos, siendo n = SOLARSYSTEM_BODYNUM + ASTEROIDS_COUNT. Debido a esto, la simulación tiene:
    UpdateOrbitalSim            -> O(n^2) : loop externo de n repeticiones, loop interno de n repeticiones
    
    UpdateOrbitalSimOptimized   -> O(n^2 - n*(n+1)/2) : loop externo [k] de n repeticiones, loop interno de n-k repeticiones.

    renderView -> O(n) :simplemente itera por todos los cuerpos y según cierto criterio los renderiza.
]

## Mejora de la complejidad computacional

[La complejidad computacional fue primero mejorada al optimizar el cálculo de las interacciones entre planetas. El algoritmo original esta el de UpdateOrbitalSim, y el optimizado es el de UpdateOrbitalSimOptimized. El optimized toma ventaja del hecho de que, entre 2 cuerpos, el vector de fuerzas que se ejercen entre si es el mismo, solo con el sentido opuesto. Entonces, cada OrbitalBody pasa a tener un vector de la fuerza total que es aplicada sobre él (El cual debe estar en {0,0,0} antes de empezar el cálculo). Un cuerpo K itera y calcula la interacción de su fuerza contra todos los cuerpos de índice K+1 hasta n. Por cada cuerpo contra el que itera, se calcula el vector de fuerzas que se suma tanto al de este cuerpo, como su contraparte invertida al cuerpo contra el que itera. Así, el proximo empieza a iterar desde K+2 hasta n. Sucesivamente, cada cuerpo K itera contra K-1 cuerpos, lo cual es una mejora sustancial del algoritmo principal, donde cada cuerpo iteraba contra n-1 cuerpos.]

## Bonus points

[
    - Hacer Jupiter 1000 veces mas masivo desestabiliza todo el Sistema Solar! Incluso puede "catapultar"   cuerpos si estos pasan lo suficientemente cerca.

    -Simular agujeros negros hacia efectos muy curiosos: Al hacer un planeta 100000 veces mas masivo, este hacía las veces de agujero negro, todos los cuerpos eran inmediatamente atraídos hacia él, y una vez cerca, eran eyectados muy lejos, para luego volver lentamente en lo que pasaban a ser trayectorias elípticas con un foco extremadamente fuerte en el planeta mas másico.

    -El Easter Egg se encontraba en la generación de los asteroides, descomentar la línea phi=0 hace que todos los asteroides comienzen en una línea recta.

]
