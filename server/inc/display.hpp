#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include "extern_libs.hpp"

# define NO_COLOR	"\033[m"

# define BLACK 		"\033[0;30m"
# define RED 		"\033[0;31m"
# define GREEN 		"\033[0;32m"
# define ORANGE 	"\033[0;33m"
# define BLUE 		"\033[0;34m"
# define PURPLE 	"\033[0;35m"
# define CYAN 		"\033[0;36m"
# define WHITE 		"\033[0;37m"

// High Intensty
# define HIGREEN 	"\033[0;92m"
# define HIORANGE 	"\033[0;93m"
# define HIPURPLE 	"\033[0;95m"

// Bold
# define BORANGE 	"\033[1;33m"
# define BBLUE 		"\033[1;34m"
# define BPURPLE 	"\033[1;35m"
# define BCYAN 		"\033[1;36m"

// Bold High Intensity
# define BHIBLACK 	"\033[1;30m"
# define BHIRED 	"\033[1;91m"
# define BHIGREEN 	"\033[1;92m"
# define BHIORANGE 	"\033[1;33m"
# define BHIYELLOW 	"\033[1;93m"
# define BHIBLUE 	"\033[1;94m"
# define BHIPURPLE 	"\033[1;35m"
# define BHIPINK 	"\033[1;95m"
# define BHICYAN 	"\033[1;96m"
# define BHIWHITE 	"\033[1;37m"

// Italic
# define ICYAN 		"\033[3;36m"

#define LINE	"\n  _____________________________________________________________________\n"
#define ENDLINE	"\n  ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n"

std::string		custom_width (int width, char c, std::string const& content);
void			display_buffer (std::vector <char>& buffer);
void			display_buffer (std::string buffer);
void	   	 	display_client_added ();
void	   	 	display_end_connexion ();
void	   	 	display_wait ();
void			log (int client_fd, std::string str);

#endif
