#include "network.h"
#include "random.h"

#include <iostream>

void Network::resize(const size_t& size){
	values.clear();
	values.resize(size);
	RNG.normal(values, 0, 1);
}

bool Network::add_link(const size_t& a, const size_t& b){
	bool has_link(false);
	std::vector<size_t> connections(neighbors(a));
	for (size_t i(0); i<connections.size(); ++i){
		if (connections[i]==b){
			has_link=true;
		}
	}
	if((a<values.size()) and (b<values.size()) and (a!=b)
		and (!has_link)){
			links.insert(std::make_pair(a,b));
			links.insert(std::make_pair(b,a));
			return true;
	}
	else return false;
}

size_t Network::random_connect(const double& mean_deg){
	links.clear();
	size_t connections(0);
	std::vector<size_t> nodes(values.size());
	for (size_t i(0); i<values.size(); ++i){
		nodes[i]=i;
	}
	RNG.shuffle(nodes);
	size_t size(nodes.size());
	for(size_t i(0); i<size; ++i){
		size_t poisson(RNG.poisson(mean_deg));
		for(size_t j(0); j<poisson; ++j){
			if(add_link(nodes.back(), nodes[j])){
				++connections;
			}	
		}		
		nodes.pop_back();
	}
	return connections;
}

size_t Network::set_values(const std::vector<double>& vec){
	size_t number_reset(0);
	for (size_t i(0); (i<vec.size()) and (i<values.size());++i){
		values[i]=vec[i];
		++number_reset;
	}
	return number_reset;
}

size_t Network::size() const{
	return values.size();
}

size_t Network::degree(const size_t & n) const{
	return links.count(n);
}

double Network::value(const size_t & n) const{
	return values[n];
}

std::vector<double> Network::sorted_values() const{
	std::vector<double>sorted(values);
	sort(sorted.rbegin(),sorted.rend());
	return sorted;
}

std::vector<size_t> Network::neighbors(const size_t& n) const{
	auto range(links.equal_range(n));
	std::vector<size_t> result;
	for (auto i(range.first); i!=range.second; ++i){
		 result.push_back(i->second);
	 }
	 sort(result.rbegin(), result.rend());
	 return result;
}
