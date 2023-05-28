import java.util.Scanner;

class iodemo
{
  public static void main(String args[])
  {
    String name;
    int id;
    Scanner in = new Scanner(System.in);
   
    System.out.println("Enter name:");
    name = in.nextLine();
    
    System.out.println("Enter id:");
    id = in.nextInt();
    
    System.out.println("--------------");
    System.out.println("Name: "+ name);
    System.out.println("ID: "+ id);
    
  }
}