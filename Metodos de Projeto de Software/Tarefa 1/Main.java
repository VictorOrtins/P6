import java.util.OptionalInt;

public class Main {
    public static void main(String[] args) {
        Bola bola = new Bola("Jabulani", (float)2.9, "Adidas");
        float valor = bola.calculaValor(10);
        System.out.println(valor);

        Esfera esfera = new Bola("Jabulani", (float)2.9, "Adidas");
        float valor2 = bola.calculaValor(10);
        System.out.println(valor);

        OptionalInt x = OptionalInt.of(5);
    }
}
