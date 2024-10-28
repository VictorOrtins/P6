public class Bola extends Esfera{
    private String nome;
    private String marca;
    public static int TESTE = 5;

    public Bola(String nome, float raio, String marca){
        super(raio);
        this.nome = nome;
        this.marca = marca;
    }

    public void setNome(String nome){
        this.nome = nome;
    }

    public void setMarca(String marca){
        this.marca = marca;
    }

    public String getNome(){
        return this.nome;
    }

    public String getMarca(){
        return this.marca;
    }

    public float calculaValor(int custoProducao){
        String teste = "" + super.raio;
        teste += this.raio;
        // teste += super.nome;

        return super.raio*custoProducao;
    }

    public float calculaValor(int custoProducao, float desconto){
        return this.raio*custoProducao*desconto;
    }

    public double calculaValor(int custoProducao, int desconto){
        return raio*custoProducao*desconto;
    }
}