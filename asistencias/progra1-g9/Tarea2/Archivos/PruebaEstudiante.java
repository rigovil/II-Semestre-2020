public class PruebaEstudiante 
{
  
  public static void main (String argv[])
  {
    Estudiante estudiante1 = new Estudiante("Luis Diego",20,95.0);
    Estudiante estudiante2 = new Estudiante("Daniela",20,100.0);
    Estudiante estudiante3 = new Estudiante("Alejandro",21,95.9);
    estudiante1.imprimirEstudiante();
    estudiante2.imprimirEstudiante();
    estudiante3.imprimirEstudiante();
    estudiante3.setNombre("Diego");
    estudiante3.setEdad(26);
    estudiante3.setPromedio(87.2657);
    estudiante3.imprimirEstudiante();
    System.out.println ("El promedio del estudiante 3 es: " + estudiante3.getPromedio());
    double promedioEstudiante2 = estudiante2.getPromedio();
    System.out.println ("El promedio del estudiante 2 es: " + promedioEstudiante2);
  }
  
}