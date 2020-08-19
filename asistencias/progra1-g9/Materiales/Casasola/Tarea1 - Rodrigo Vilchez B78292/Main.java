public class Main {
   public static void main(String argv[]){
		Habitacion habitacion1 = new Habitacion();
		habitacion1.setNumero(001);
		habitacion1.setTipo("Doble");
		habitacion1.setTv("Si");
		habitacion1.setAc("No");
		habitacion1.setWifi("Si");
		
		System.out.println(habitacion1.toString());
		
		Habitacion habitacion2 = new Habitacion();
		habitacion2.setNumero(12);
		habitacion2.setTipo("Sencilla");
		habitacion2.setTv("No");
		habitacion2.setAc("Si");
		habitacion2.setWifi("Si");
		
		System.out.println(habitacion2.toString());
		System.exit(0);
}
}