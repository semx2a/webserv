/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:47:12 by seozcan           #+#    #+#             */
/*   Updated: 2023/09/20 17:27:50 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_HPP
# define PRINT_HPP

# include <vector>
# include <map>
# include <iostream>
# include <iomanip>
# include <iostream>
# include <string>
# include <sstream>

# define NO_COLOR	"\033[m "

# define BLACK 		"\033[0;30m "
# define RED 		"\033[0;31m "
# define GREEN 		"\033[0;32m "
# define ORANGE 	"\033[0;33m "
# define BLUE 		"\033[0;34m "
# define PURPLE 	"\033[0;35m "
# define CYAN 		"\033[0;36m "
# define WHITE 		"\033[0;37m "

// High Intensty
# define HIGREEN 	"\033[0;92m "
# define HIORANGE 	"\033[0;93m "
# define HIPURPLE 	"\033[0;95m "

// Bold
# define BORANGE 	"\033[1;33m "
# define BBLUE 		"\033[1;34m "
# define BPURPLE 	"\033[1;35m "
# define BCYAN 		"\033[1;36m "

// Bold High Intensity
# define BHIBLACK 	"\033[1;30m "
# define BHIRED 	"\033[1;91m "
# define BHIGREEN 	"\033[1;92m "
# define BHIORANGE 	"\033[1;33m "
# define BHIYELLOW 	"\033[1;93m "
# define BHIBLUE 	"\033[1;94m "
# define BHIPURPLE 	"\033[1;35m "
# define BHIPINK 	"\033[1;95m "
# define BHICYAN 	"\033[1;96m "
# define BHIWHITE 	"\033[1;37m "

// Italic
# define ICYAN 		"\033[3;36m "


void			log (int client_fd, std::string str);
std::string		str_of(size_t size, const std::string & c);
std::string		custom_width (int width, char c, std::string const& content);

std::string 	print_vector_of_char (std::vector <char>& buffer);
std::string 	print_str (std::string buffer);

std::string	   	print_client_added ();
std::string	   	print_end_connexion ();
std::string	   	print_wait ();

template <typename T>
std::string print_vector(std::vector<T> const& vec);

template <typename T>
std::string	 print_classVector(std::vector<T> const& vec);

template <typename KeyType, typename ValueType>
std::string print_map(const std::map<KeyType, ValueType>& myMap);

template <typename KeyType, typename T>
std::string print_map_of_vectors(const std::map<KeyType, T>& myMap);

# include "print.tpp"

#endif
