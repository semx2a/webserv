/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:47:12 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/12 15:34:51 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_HPP
# define PRINT_HPP

# include <vector>
# include <map>
# include <iostream>

# include "display.hpp"

template <typename T>
void printVector(const std::vector<T>& vec);

template <typename KeyType, typename ValueType>
void printMap(const std::map<KeyType, ValueType>& myMap);

# include "../templates/print.tpp"

#endif
