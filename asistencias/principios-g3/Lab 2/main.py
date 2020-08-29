#Este es un comentario que nos sirve para dar contexto del programa
#Podemos tener varias líneas en un comentario
print("Digite su nombre:")
nombre = input()
print("Bienvenido "+nombre) # Los comentarios tambien pueden ir acá
print("La función input nos permite leer texto\nNo importa que haya sido un número lo que escribió el usuario")

print("Digite un número")
numeroDigitado = input()
print(numeroDigitado)
print("El tipo de datos del dato digitado es "+str(type(numeroDigitado)))
#resultado = numeroDigitado + 2
print("Por más que el string contiene un número no es un número")
numero1 = int(numeroDigitado)
print("El tipo de datos del dato ahora es {}".format(type(numero1)))
#Ahora si es un número, por eso podemos hacer aritmética
resultado1 = numero1 ** 2
print(resultado1)