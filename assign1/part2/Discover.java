import java.io.*;
import java.util.*;

public class Discover {

	private static void etchostParser(ArrayList<String> hostNames){
		FileReader fileReader;
		BufferedReader bufferedReader;
		String line;
		try {
			fileReader = new FileReader("/etc/hosts");
			bufferedReader = new BufferedReader(fileReader);
			while((line = bufferedReader.readLine()) != null){
				line = line.trim();
				if (line.startsWith("#") || line.isEmpty())
					continue;
				else if (line.contains("\t") && line.split("\t").length>1 && !line.split("\t")[1].isEmpty())
					hostNames.add(line.split("\t")[1]);
				else if (line.contains(" ") && line.split(" ").length>1 && !line.split(" ")[1].isEmpty())
					hostNames.add(line.split(" ")[1]);
			}
			bufferedReader.close();
			fileReader.close();
		}
		catch (Exception e) {System.out.println("etchostParser Exception caught" + e);}
	}

	private static void sshconfigParser(ArrayList<String> hostNames, String filePath){
		FileReader fileReader;
		BufferedReader bufferedReader;
		String line;
		try {
			fileReader = new FileReader(filePath);
			bufferedReader = new BufferedReader(fileReader);
			while((line = bufferedReader.readLine()) != null){
				line = line.trim();
				if (line.startsWith("#") || line.isEmpty())
					continue;
				else if(line.startsWith("Host") && !line.contains("=")){
					line = line.trim().split(" ")[1];
					char firstChar = line.charAt(0);
					if ((firstChar>='A' && firstChar<='Z')||(firstChar>='a' && firstChar<='z'))
						hostNames.add(line);
				}
				else if(line.startsWith("Host") && line.contains("=")){
					line = line.trim().split("=")[1];
					char firstChar = line.charAt(0);
					if ((firstChar>='A' && firstChar<='Z')||(firstChar>='a' && firstChar<='z'))
						hostNames.add(line);
				}

			}
			bufferedReader.close();
			fileReader.close();
		}
		catch (Exception e) {/*System.out.println("sshconfigParser Exception caught" + e);*/}
	}

	public static void main(String args[]){

		/*
		 * Read etc/passwd file and get all user directories
		 * for each diretory
		 1)	etc/hosts
		 2) ~/.ssh/config and etc/ssh/ssh_config
		 3) ~/.ssh/authorized_keys 
		 4) ~/.ssh/known_hosts and etc/ssh/ssh_known_hosts
		 */

		ArrayList<String> homeDirPaths = new ArrayList<String>();
		ArrayList<String> discHostNames = new ArrayList<String>();
		String line;
		FileReader fileReader;
		BufferedReader bufferedReader;

		// fetch all the user directories
		try {
			fileReader = new FileReader("/etc/passwd");
			bufferedReader = new BufferedReader(fileReader);

			while((line = bufferedReader.readLine()) != null){
				homeDirPaths.add(line.split(":")[5]);
			}

			bufferedReader.close();
			fileReader.close();
		}
		catch (Exception e) {}

		//parsing the /etc/hosts
		etchostParser(discHostNames);


		//parsing the /etc/ssh/ssh_config file
		sshconfigParser(discHostNames, "/home/vignesh/Documents/softwareSec/assign1/part2/test/mhd/sshconfig");
		//parshing the home/.ssh/ssh_config
		for (String path : homeDirPaths)
			sshconfigParser(discHostNames, path+"/.ssh/sshconfig");


		//parsing the ~/.ssh/authorized_keys


		// printing all the hostnames
		for (String path : discHostNames)
			System.out.println(path);
	}
}
