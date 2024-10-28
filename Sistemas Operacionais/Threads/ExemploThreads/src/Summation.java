public class Summation implements Runnable{
    private int upper;
    private int sum;

    public Summation(){
        this.upper = 0;
        this.sum = 0;
    }

    public Summation(int upper){
        this.upper = upper;
        this.sum = 0;
    }

    @Override
    public void run() {
        this.sum = 0;
        for(int i = 0; i < this.upper; i++){
            this.sum += i;
        }
    }

    public int getUpper() {
        return this.upper;
    }

    public void setUpper(int upper) {
        this.upper = upper;
    }

    public int getSum() {
        return this.sum;
    }

    public void setSum(int sum) {
        this.sum = sum;
    }


    

}


