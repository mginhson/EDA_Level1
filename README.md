# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

*
Matteo Ginhson: [Realizo gran parte del codigo encargado de mover los cuerpos y los asteroides]
Nicanor Otamendi: [Realizo la mayor parte del codigo visual y la creacion de asteroides]

[De todas formas al ser un grupo de solo 2 personas, ambos trabajamos sobre todas las partes del codigo, en su mayoria de forma sincronica mediante Liveshare y asincronica GitHub.]

## Verificación del timestep

[Si bien al ser las derivadas discretas, reducir el timestep mejoraría la precisión, mas aumentarlo mejoraría la velocidad de la simulación. Intentamos aumentarlo lo máximo posible, pero notamos que con valores rondando los 500 * SECONDS_PER_DAY, la simulación era muy inexacta (Mercurio salia eyectado por el Sol en vez de mantenerse en su órbita). Así que preferimos mantener la fidelidad de la simulación ejecutando 100 días por UpdateOrbitalSimOptimized (El valor por defecto). Al testear este timestep, la simulación sigue siendo suficientemente precisa luego de mucho más que 100 años, lo cúal es suficiente para nuestros fines.]

## Verificación del tipo de datos float

[Luego de evaluar la precision que brindaba este tipo de datos y los recursos de las computadoras, consideramos mejor trabajar con tipo de dato double . De esta forma, aumenta la precision de la simulacion reduciendo el impacto de los errores de manera significativa cuando pasa mucho tiempo dentro de la simulacion, sumado al hecho de que nos sobran ampliamente los recursos para hacer este cambio. ]

## Complejidad computacional con asteroides

[En nuestra implementacion inicial, buscndo el mayor realismo posible, se calcula la interacción de todos los cuerpos contra todos, teniendo en total n elementos, siendo n = SOLARSYSTEM_BODYNUM + ASTEROIDS_COUNT. Debido a esto, la simulación tiene:
    UpdateOrbitalSim            -> O(n^2) : loop externo de n repeticiones, loop interno de n repeticiones
    


Para mejorar aun mas el rendimiento, y al ver que la interaccion entre asteroides y planetas apenas modifica en unos pocos metros cada anos las orbitas, decidimos que los asteroides solo puedan recibir fuerzas y no ejercerlas, ya que dado a que su magnitud es minuscula comparada con la de los planetas, estos apenas modificarian las orbitas.
De este modo:
    UpdateOrbitalSimOptimized -> O(n), haciendolo mucho mas eficiente que su version anterior.
]

## Mejora de la complejidad computacional

[
    Al utilizar el nuevo algoritmo (Hacer que los cuerpos que afectan las orbitas de los demas son solo los planetas), la 
    complejidad computacional pasa de ser O(n^2) a O(n). 
    Otra pequenia mejora que hicimos fue, en View, solo dibujar como esfera los asteroides que estan lo suficientemente 
    cerca de la camara, siendo puntos si no cumplen esta condicion. Esto mejoro enormemente la medicion de FPS, indicando que este era un cuello de botella grafico. 
]



## Bonus points

[
    - Hacer Jupiter 1000 veces mas masivo desestabiliza todo el Sistema Solar! Incluso puede "catapultar"   cuerpos si estos pasan lo suficientemente cerca.

    -Se puede activar la simulacion de Alpha Centauri llamando a constructOrbitalSim_BONUS

    -Simular agujeros negros hacia efectos muy curiosos: Al hacer un planeta 100000 veces mas masivo, este hacía las veces de agujero negro, todos los cuerpos eran inmediatamente atraídos hacia él, y una vez cerca, eran eyectados muy lejos, para luego volver lentamente en lo que pasaban a ser trayectorias elípticas con un foco extremadamente fuerte en el planeta mas masivo.

    -El Easter Egg se encontraba en la generación de los asteroides, descomentar la línea phi=0 hace que todos los asteroides comienzen en una línea recta.

]
