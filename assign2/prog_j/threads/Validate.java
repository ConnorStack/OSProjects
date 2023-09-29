package prog_j.threads;

public class Validate {
    
    private static int argValue;

    public static boolean isArgsValid(String[] args){
        boolean valid = false;

        if(isLengthValid(args.length)){
            String integerRepresentation = args[0];
            try{
                int intValue = Integer.parseInt(integerRepresentation);
                if(isPositive(intValue)){
                    argValue = intValue;
                    valid = true;
                }else{
                    System.out.println("Value must be positive");
                }
            }catch (NumberFormatException e) {
                System.out.println("Argument must be an integer");
            }
            
        }

        return valid;
    }

    public static int getArgValue(){
        return argValue;
    }

    private static boolean isLengthValid(int length){
        return (length == 1) ? true : false;
    }

    private static boolean isPositive(int arg){
        return (arg > 0) ? true : false;
    }
}
