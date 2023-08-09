/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:44:00 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/09 17:48:35 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/print.hpp"

template <typename T>
void printVector(const std::vector<T>& vec) {
	
    typename std::vector<T>::const_iterator it;
    
	for (it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    
	std::cout << std::endl;
}
