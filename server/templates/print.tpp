/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:44:00 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/16 17:48:25 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/print.hpp"

template <typename T>
void printVector(const std::vector<T>& vec) {
	
    typename std::vector<T>::const_iterator it;
    
	for (it = vec.begin(); it != vec.end(); ++it) {
        
        std::cout << *it << std::endl;
    }
    
	std::cout << std::endl;
}

template <typename KeyType, typename ValueType>
void printMap(const std::map<KeyType, ValueType>& myMap) {

    typename std::map<KeyType, ValueType>::const_iterator it;
    
    for (it = myMap.begin(); it != myMap.end(); ++it) {
        
        std::cout << RED << it->first << std::endl;
        
        std::cout << GREEN;
        printVector(it->second);
        std::cout << NO_COLOR << std::endl;
    }
}
