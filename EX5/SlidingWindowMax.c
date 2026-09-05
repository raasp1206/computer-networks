from collections import deque

class Solution(object):
    def maxSlidingWindow(self, nums, k):
        if not nums or k == 0:
            return []
        
        result = []
        window = deque()
        
        for i, num in enumerate(nums):
            if window and window[0] < i - k + 1:
                window.popleft()
                
            while window and nums[window[-1]] < num:
                window.pop()
                
            window.append(i)
            
            if i >= k - 1:
                result.append(nums[window[0]])
                
        return result
