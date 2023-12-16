#include "utils.hpp"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ans;

	ans = (unsigned char *)s;
	while (n--)
	{
		*ans = (unsigned char)c;
		ans++;
	}
	return (s);
}