/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:47:12 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/10 15:59:10 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_HPP
# define PRINT_HPP

# include <vector>
# include <map>
# include <iostream>

#include "display.hpp"

# include "../templates/print.tpp"

template <typename T>
void printVector(const std::vector<T>& vec);

template <typename KeyType, typename ValueType>
void printMap(const std::map<KeyType, ValueType>& myMap);

#endif