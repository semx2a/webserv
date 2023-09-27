/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:44:00 by seozcan           #+#    #+#             */
/*   Updated: 2023/09/23 23:03:22 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "print.hpp"

template <typename T>
std::string	 print_vector(std::vector<T> const& vec) {
	
    typename std::vector<T>::const_iterator it;
	std::stringstream 						sstream;
    
	for (it = vec.begin(); it != vec.end(); ++it) {
        
        sstream << *it;
		if (it + 1 != vec.end())
			sstream << ", ";
    }
	return sstream.str();
}

template <typename T>
std::string	 print_classVector(std::vector<T> const& vec) {
	
    typename std::vector<T>::const_iterator	it;
	std::stringstream 						sstream;
    
	for (it = vec.begin(); it != vec.end(); ++it) {
        
		sstream << BLUE <<"---- vec[" << it - vec.begin() << "] ----" << std::endl;
        sstream << *it;
		sstream << std::endl;
    }
	return sstream.str();
}

template <typename KeyType, typename ValueType>
std::string	 print_map(std::map<KeyType, ValueType> const& myMap) {

	typename std::map<KeyType, ValueType>::const_iterator	it;
	std::stringstream 										sstream;

	sstream << std::endl;
	for (it = myMap.begin(); it != myMap.end(); ++it) {
		sstream << std::setw(27) << "map[" << it->first << "] = " << it->second; 
		if (it != myMap.end() && it != --myMap.end()) 
			sstream << std::endl;
	}
	return sstream.str();
}

template <typename KeyType, typename T>
std::string	 print_map_of_vectors(std::map<KeyType, T> const& myMap) {

    typename std::map<KeyType, T>::const_iterator	it;
	std::stringstream 								sstream;
    
    for (it = myMap.begin(); it != myMap.end(); ++it) {
        
        sstream << RED << it->first << std::endl;
        
        sstream << GREEN;
        print_vector(it->second);
        sstream << NO_COLOR << std::endl;
    }
	return sstream.str();
}
