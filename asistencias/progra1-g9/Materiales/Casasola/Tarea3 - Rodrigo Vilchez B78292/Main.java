import java.util.Scanner;

public class Main {
	public static void main (String [] hola) {
		Scanner entrada = new Scanner(System.in);
		System.out.print("Digite:\n 1 si es de m3 a L.\n 2 si es de L a mL.\n 3 si es de L a cm3.\n 4 si es de L a m3\n");
		double conversion = entrada.nextDouble();
		if(conversion > 4 || conversion < 1){
				System.out.printf("No es un digito valido.");
			}
		else {
			System.out.print("Digite el primer valor:");
			double valor = entrada.nextDouble();
		
			if(conversion == 1){
				double resultado = valor / 0.001;
				System.out.printf("La conversion es: "+resultado+" Litros.");
			}
			else {
				if(conversion == 2){
					double resultado = valor * 1000;
					System.out.printf("La conversion es: " +resultado+" mL.");
				}
				else {
					if(conversion == 3){
						double resultado = valor * 1000;
						System.out.printf("La conversion es "+resultado+" cm3.");
					}
					else {
						if(conversion == 4){
							double resultado = valor / 1000;
							System.out.printf("La conversion es "+resultado+" m3.");
						}
					}
				}
			}
		}
		System.exit(0);
	}
}
	
			
	