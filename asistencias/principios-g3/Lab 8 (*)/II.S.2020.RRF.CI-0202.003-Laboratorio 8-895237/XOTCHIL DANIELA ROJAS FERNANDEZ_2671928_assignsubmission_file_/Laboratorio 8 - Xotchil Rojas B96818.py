# -*- coding: utf-8 -*-
"""
Created on Sat Oct  3 12:52:53 2020

@author: Windows 10
"""

### Problema 1 ###

#Calculadora
#Cinco opciones:
#Sumar dos números positivos
#Restar dos números positivos
#Multiplicar dos números positivos
#Dividir dos números positivos
#Salir 

#Métodos

#Sumar 
def sumar(primer_valor, segundo_valor):
  resultado = primer_valor + segundo_valor
  return resultado 

#Restar
def restar(primer_valor, segundo_valor):
  resultado = primer_valor - segundo_valor
  return resultado

#Multiplicar
def multiplicar(primer_valor, segundo_valor):
    resultado = primer_valor * segundo_valor
    return resultado

#Dividir
def dividir(primer_valor, segundo_valor):
    resultado = primer_valor / segundo_valor
    return resultado

#Programa principal 

print("Bienvenido al programa de calculadora")

terminar_programa = False

while terminar_programa == False:
  #Desplegar menú
  print("\nMenú de opciones\n")
  print("1. Sumar\n2. Restar\n3. Multiplicar\n4. Dividir\n5. Salir")
  
  try:
    operacion_usuario = int(input("Ingrese la operación que desea realizar: "))
    if operacion_usuario == 1:

      #Suma
      print("\nSUMA\n")
      #Solicitar los valores al usuario
      
      numeros_validos = False
      
      while numeros_validos == False:
        try:
          primer_numero = float(input("Ingrese el primer valor: "))
          if primer_numero > 0: 
            segundo_numero = float(input("Ingrese el segundo valor: "))
            if segundo_numero > 0:
              numeros_validos = True
              resultado_suma = sumar(primer_numero, segundo_numero)
              print("\nEl resultado de la suma es: ",resultado_suma)
            else:
              print("El número ingresado debe ser mayor a cero")
          else:
            print("El número ingresado debe ser mayor a cero") 
        except ValueError:
          print("El valor ingresado no es permitido")

    elif operacion_usuario == 2:
      #Resta
      print("\nRESTA\n")
      #Solicitar los valores al usuario
      
      numeros_validos = False
      
      while numeros_validos == False:
        try:
          primer_numero = float(input("Ingrese el primer valor: "))
          if primer_numero > 0: 
            segundo_numero = float(input("Ingrese el segundo valor: "))
            if segundo_numero > 0:
              numeros_validos = True
              resultado_resta = restar(primer_numero, segundo_numero)
              print("\nEl resultado de la resta es: ",resultado_resta)
            else:
              print("El número ingresado debe ser mayor a cero")
          else:
            print("El número ingresado debe ser mayor a cero") 
        except ValueError:
          print("El valor ingresado no es permitido")
    
    elif operacion_usuario == 3:
      #Multiplicación
      print("\nMULTIPLICACIÓN\n")
      #Solicitar los valores al usuario
      
      numeros_validos = False
      
      while numeros_validos == False:
        try:
          primer_numero = float(input("Ingrese el primer valor: "))
          if primer_numero > 0: 
            segundo_numero = float(input("Ingrese el segundo valor: "))
            if segundo_numero > 0:
              numeros_validos = True
              resultado_multiplicacion = multiplicar(primer_numero, segundo_numero)
              print("\nEl resultado de la multiplicación es: ",resultado_multiplicacion)
            else:
              print("El número ingresado debe ser mayor a cero")
          else:
            print("El número ingresado debe ser mayor a cero") 
        except ValueError:
          print("El valor ingresado no es permitido")
    
    elif operacion_usuario == 4:
      #División
      print("\nDIVISIÓN\n")
      print("*Introduzca el dividendo como primer número y el divisor como segundo número*")
      #Solicitar los valores al usuario
      
      numeros_validos = False
      
      while numeros_validos == False:
        try:
          primer_numero = float(input("Ingrese el primer valor: "))
          if primer_numero > 0: 
            segundo_numero = float(input("Ingrese el segundo valor: "))
            if segundo_numero > 0:
              numeros_validos = True
              resultado_division = dividir(primer_numero, segundo_numero)
              print("\nEl resultado de la división es: ",resultado_division)
            elif segundo_numero == 0:
                print("No es posible dividir entre 0")
            else:
              print("El número ingresado debe ser mayor a cero")
          else:
            print("El número ingresado debe ser mayor a cero") 
        except ValueError:
          print("El valor ingresado no es permitido")
    
    elif operacion_usuario == 5:
      terminar_programa = True
    else:
      print("La operación indicada no es correcta")
  except ValueError:
    print("El valor ingresado no es permitido")
    
    
    
### Problema 2 ###
    
#Método
def numero_mas_cercano(primer_numero,segundo_numero,tercer_numero):
    
    #los parametros se convierten a un numero entero para el correcto funcionamiento del código contador de distancia
    objetivo = int(primer_numero)
    valor1 = int(segundo_numero)
    valor2 = int(tercer_numero)
    
    #contador de distancia entre el objetivo y los otros dos numeros
    if objetivo < valor1:
        contador_1 = 0
        for elemento in range(objetivo,valor1):
            contador_1 += 1
            
    if valor1 < objetivo:
        contador_1 = 0
        for elemento in range(valor1,objetivo):
            contador_1 += 1
    
    if objetivo < valor2:
        contador_2 = 0
        for elemento in range(objetivo,valor2):
            contador_2 += 1
            
    if valor2 < objetivo:
        contador_2 = 0
        for elemento in range(valor2,objetivo):
            contador_2 += 1
       
    #valores para el retorno del resultado
    numero_objetivo = primer_numero
    primer_numero = segundo_numero
    segundo_numero = tercer_numero
    
    #retorno del valor más cercano al numero objetivo
    if contador_1 < contador_2:
        return primer_numero
    elif contador_2 < contador_1:
        return segundo_numero
    elif contador_1 == contador_2:
        if numero_objetivo < primer_numero < segundo_numero:
            return primer_numero
        elif numero_objetivo < segundo_numero < primer_numero:
            return segundo_numero
        elif segundo_numero < primer_numero < numero_objetivo:
            return primer_numero
        elif primer_numero < segundo_numero < numero_objetivo:
            return segundo_numero
    


print("\nA continuación se le presenta un programa que le solicita tres numeros y le retornará el número más cercano al valor objetivo.")

#Programa principal
try:
    numero_objetivo = float(input("Introduzca el valor objetivo: "))
    primer_numero_a_evaluar = float(input("Introduzca el primer número a evaluar: "))
    segundo_numero_a_evaluar = float(input("Introduzca el segundo número a evaluar: "))
    
    resultado = numero_mas_cercano(numero_objetivo,primer_numero_a_evaluar,segundo_numero_a_evaluar)
    print("\nEl número más cercano al valor objetivo es:",resultado)
    
except ValueError:
    print("El dato brindado es incorrecto.\nGracias por utilizar el programa")


### Problema 3 ###

def numero_entero():
    dato_introducido = input("Introduzca un dato: ")
    
    dato_entero = False
    while dato_entero == False:
        if dato_introducido == int:
            if dato_introducido <= 0:
                print("Siga intentando.")
            elif dato_introducido > 0:
                dato_entero = True
                print("Usted introdujo el entero",dato_introducido)
        elif dato_introducido == float:
            print("Siga intentando.")
        elif dato_introducido == str:
            print("Siga intentando.")

print("\nA continuación introduzca un dato, cuando usted indique el dato correcto se le avisará. Pueden ser números o letras.\n")
numero_entero()
        
    





