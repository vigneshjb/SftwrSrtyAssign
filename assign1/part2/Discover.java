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
		catch (Exception e) {/*System.out.println("etchostParser Exception caught" + e);*/}
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

	private static void authorizedKeysParser(ArrayList<String> hostNames, String filePath){
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
				else if(line.startsWith("from") && line.contains("=")){
					String origline = line;
					line = line.trim().split(" ")[0];
					line = line.split("=")[1];
					line = line.substring(1, line.length()-1);
					if (line.contains(",")){
						hostNames.add(line.split(",")[0]);
						hostNames.add(line.split(",")[1]);
					}
					else
						hostNames.add(line);
					if (origline.contains("@"))
						hostNames.add(origline.split("@")[1]);
				}
				else if(line.contains("@")){
					line = line.trim().split("@")[1];
					if (!line.contains(" "))
						hostNames.add(line);
					else{
						line = line.split(" ")[0];
						hostNames.add(line);
					}
				}
			}
			bufferedReader.close();
			fileReader.close();
		}
		catch (Exception e) {/*System.out.println("authorizedKeysParser Exception caught" + e);*/}
	}

	private static void knownHostsParser(ArrayList<String> hostNames, String path){
		FileReader fileReader;
		BufferedReader bufferedReader;
		String line;
		try {
			fileReader = new FileReader(path);
			bufferedReader = new BufferedReader(fileReader);
			while((line = bufferedReader.readLine()) != null){
				line = line.trim();
				if (line.startsWith("#") || line.isEmpty())
					continue;
				else if (line.startsWith("|"))
					continue;
				else {
					line = line.split(" ")[0];
					if (line.contains(",")){
						String items[] = line.split(",");
						for (String item : items){
							char firstChar = item.charAt(0);
							if ((firstChar>='A' && firstChar <='Z')||(firstChar>='a'&& firstChar<='z'))
								System.out.println(item);
						}
					}
					else {
						char firstChar = line.charAt(0);
						if ((firstChar>='A' && firstChar <='Z')||(firstChar>='a'&& firstChar<='z'))
								System.out.println(line);
					}
				}
			}
			bufferedReader.close();
			fileReader.close();
		}
		catch (Exception e) {/*System.out.println("knownHostsParser Exception caught" + e);*/}
	}

	public static void main(String args[]){

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
		// sshconfigParser(discHostNames, "/home/vignesh/Documents/softwareSec/assign1/part2/test/mhd/sshconfig");
		sshconfigParser(discHostNames, "/etc/ssh/ssh_config");
		//parshing the home/.ssh/ssh_config
		for (String path : homeDirPaths)
			sshconfigParser(discHostNames, path+"/.ssh/config");

		//parsing the ~/.ssh/authorized_keys
		for (String path : homeDirPaths)
			authorizedKeysParser(discHostNames, path+"/.ssh/authorized_keys");
		// authorizedKeysParser(discHostNames, "/home/vignesh/Documents/softwareSec/assign1/part2/test/mhd/authorizedkeys");
		// authorizedKeysParser(discHostNames, "/home/vignesh/Documents/softwareSec/assign1/part2/test/revanth/authorized_keys");
		// authorizedKeysParser(discHostNames, "/home/vignesh/Documents/softwareSec/assign1/part2/test/adam/authorized_keys");

		//parsing the known_hosts
		knownHostsParser(discHostNames, "/etc/ssh/ssh_known_hosts");
		for (String path : homeDirPaths)
			knownHostsParser(discHostNames, path+"/.ssh/known_hosts");
		// knownHostsParser(discHostNames, "/home/vignesh/Documents/softwareSec/assign1/part2/test/adam/known_hosts");
		// knownHostsParser(discHostNames, "/home/vignesh/Documents/softwareSec/assign1/part2/test/mhd/knownhosts");
		// knownHostsParser(discHostNames, "/home/vignesh/Documents/softwareSec/assign1/part2/test/revanth/known_hosts");

		// printing all the hostnames
		for (String path : discHostNames)
			System.out.println(path);
	}
}
