import java.util.InputMismatchException;
import java.util.Scanner;

public class App {
    public static void main(String[] args) throws Exception {
        System.out.println("--- TESTE ---");
        try (Scanner scan = new Scanner(System.in)) {
            int upper;
            while(true){
                System.out.print("Digite um número para ser o upper da soma: ");
                upper = scan.nextInt();
                try{

                }
                catch(InputMismatchException e){
                    System.out.println("Digite um número válido!\n");
                    continue;
                }

                break;
            }
            
            Summation sumObject = new Summation(upper);
            Thread thread = new Thread(sumObject);
            thread.start();
            try{
                thread.join();
                System.out.printf("A soma até %d é de: %d\n", sumObject.getUpper(), sumObject.getSum());
            }
            catch(InterruptedException e){
                System.err.println("Algo deu errado!");
            }
        }
    }
}
