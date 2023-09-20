/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:44:00 by seozcan           #+#    #+#             */
/*   Updated: 2023/09/20 11:28:52 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "print.hpp"

template <typename T>
std::string	 print_vector(const std::vector<T>& vec) {
	
    typename std::vector<T>::const_iterator it;
	std::stringstream 						sstream;
    
	for (it = vec.begin(); it != vec.end(); ++it) {
        
        sstream << *it;
    }
	return sstream.str();
}

template <typename KeyType, typename ValueType>
std::string	 print_map(const std::map<KeyType, ValueType>& myMap) {

	typename std::map<KeyType, ValueType>::const_iterator	it;
	std::stringstream 										sstream;
	
	for (it = myMap.begin(); it != myMap.end(); ++it) {
		sstream << "map[" << it->first << "] = " << it->second << " ";
	}
	return sstream.str();
}

template <typename KeyType, typename T>
std::string	 print_map_of_vectors(const std::map<KeyType, T>& myMap) {

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
