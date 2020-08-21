// Hola esto es un comentario de una linea

/*
 Esto es un comentario
 de varias lineas
 */

public class Estudiante
{
  String nombre; // Este atributo representa el nombre del estudiante.
  int edad; // Esta es la edad del estudiante y es un entero.
  double promedio; // Este es el promedio, un numero real.
  
  //En este metodo se asignan valores a la clase estudiante al ser creada 
  public Estudiante (String n, int e, double p)
  {
    nombre = n;
    edad = e;
    promedio = p;
  }

  //Este metodo asigna el nombre al estudiante
  public void setNombre (String nombre)
  {
    this.nombre = nombre; 
  }
  
  public String getNombre ()
  {
    return this.nombre;
  }
  
  public int getEdad()
  {
    return this.edad;
  }
  
  public double getPromedio()
  {
    return this.promedio;
  }
  
  public void setEdad(int edadParametro)
  {
    edad = edadParametro;
  }
  
  public void setPromedio (double promedioParametro)
  {
    promedio = promedioParametro;
  }
  
  // este metodo imprime los atributos de la clase estudiante
  public void imprimirEstudiante ()
  {
    System.out.println("Nombre: " + nombre); 
    System.out.println("Edad: " + edad);
    System.out.println("Promedio: " + promedio + "\n");
  }
}