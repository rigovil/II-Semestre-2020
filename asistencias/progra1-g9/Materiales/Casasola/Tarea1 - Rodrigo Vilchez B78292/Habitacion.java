public class Habitacion {
	private int numero;
	private String tipo;
	private String tv;
	private String ac;
	private String wifi;
	
	public String getWifi() {
		return wifi;
	}
	
	public int getNumero() {
		return numero;
	}
	
	public String getTipo() {
		return tipo;
	}
	
	public String getTv() {
		return tv;
	}
	
	public String getAc() {
		return ac;
	}
	
	public void setWifi(String wifi){
	this.wifi = wifi;
    }

	public void setNumero(int numero){
		this.numero = numero;
	}
	
	public void setTipo (String tipo){
		this.tipo = tipo;
	}
	
	public void setTv (String tv){
		this.tv = tv;
	}
	
	public void setAc (String ac){
		this.ac = ac;
	}
	
	public String toString () {
		return "Numero de habitacion: " + numero + " - Tipo de habitacion: " + tipo + " - Television: " + tv + " - AC: " + ac + " - WiFi: " + wifi + ".";
	}
}