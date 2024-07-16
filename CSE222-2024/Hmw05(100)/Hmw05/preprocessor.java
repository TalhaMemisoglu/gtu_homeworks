public class preprocessor {
	private String initial_string;
	private String preprocessed_string;
		
	public preprocessor(String str) {
		initial_string = str;
		preprocessed_string = "";
	}

	public void preprocess() {
		// do not edit this method
		capitalize();
		clean();
	}
	
	private void capitalize() {
		initial_string = initial_string.toUpperCase();
		initial_string = initial_string.replaceAll("İ", "I");
	}

	private void clean() {
		for(int i=0; i < initial_string.length(); i++){
			Character c = initial_string.charAt(i);
			if(Character.isLetter(c)){
				preprocessed_string += c;
			}
		}
	}
	
	public String get_preprocessed_string() {
		return preprocessed_string;
	}
}